import roslib;roslib.load_manifest('mission_planner')
import rospy
import smach
import smach_ros
import header
import ip_header
import main
from kraken_msgs.msg._controllerGoal import controllerGoal
from smach_ros.simple_action_state import SimpleActionState
from ip_msgs.msg._buoyAction import buoyAction
from ip_msgs.msg._buoyGoal import buoyGoal
from kraken_msgs.msg._controllerAction import controllerAction
from actionlib.simple_action_client import SimpleActionClient
from actionlib_msgs.msg._GoalStatus import GoalStatus
from main import Interaction
from kraken_msgs.msg._ipControllererror import ipControllererror


class DetectingBuoy(smach.State):
    def __init__(self,resources):
        """
        This state for just detecting the buoy  
        and intiialzing buoy parameters and then searching buoy
        """
        smach.State.__init__(self, outcomes=['buoy_found','time_out'],input_keys=['depth','threshold'])
        self.resources=resources
    def execute(self, ud):
        #comment this at last
        #self.resources=Interaction()
        #-------------
        
        
        rospy.loginfo("[TP]...Starting buoy detecting task...")
        
        """
        This is a concurrence container containing the SimpleActionState for both controllerSevrer and buoyServer
        the outcome map specifices what to do when one of the states of cm reach a final outcome
        Everytime the controller goal is provided using input key of the state
        
        """
        cm=smach.Concurrence(outcomes=['buoy_found','buoy_unfound','time_out'], 
                            default_outcome='buoy_unfound', 
                            outcome_map={
                                         'buoy_found':{'DETECT_SERVER':'succeeded'},
                                         'time_out':{'CONTROLLER_SERVER':'succeeded'}
                                         },
                            input_keys=['u_r','u_p','u_y','u_d'])
        cm.userdata.u_r=None
        cm.userdata.u_p=None
        cm.userdata.u_y=None
        cm.userdata.u_d=None
        
        with cm:
            goalb=buoyGoal()
            goalb.order=ip_header.ALLIGN_BUOY
            smach.Concurrence.add('DETECT_SERVER', SimpleActionState(header.BUOY_DETECT_ACTION_SERVER, 
                                                                 buoyAction, goal))
            
            smach.Concurrence.add('CONTROLLER_SERVER',
                                  SimpleActionState(header.CONTROLLER_SERVER,
                                                    controllerAction,
                                                    goal_slots=['r','p','y','d']),
                                  remapping={
                                             'r':'u_r','p':'u_p','y':'u_y','d':'u_d'
                                             }
                                  )
        
        
        
        """
        activating the buoy and going to a specific depth
        """
        cm.userdata.u_r=self.resources.pose[header.PoseEnum.roll]
        cm.userdata.u_d=self.tDepth
        cm.userdata.u_p=self.resources.pose[header.PoseEnum.pitch]
        cm.userdata.u_y=self.resources.pose[header.PoseEnum.yaw]
        rospy.loginfo("moving to depth %f",self.tDepth)
        
        
        outcome=cm.execute()
        
        if(outcome=='time_out'):
            rospy.loginfo("succesfully reached depth %f ",self.tDepth)
        elif outcome=='buoy_found':
            rospy.loginfo("Found buoy while reaching depth %d !!",self.tDepth)
            return 'buoy_found'
        
        
        
        """
           now setting yaw -45 degrees and checking for buoy within this range  
        """
        cm.userdata.u_r=self.resources.pose[header.PoseEnum.roll]
        cm.userdata.u_d=self.resources.pose[self.tDepth]
        cm.userdata.u_p=self.resources.pose[header.PoseEnum.pitch]
        cm.userdata.u_y=-45
        rospy.loginfo("turning --45 degrees left ")
        outcome=cm.execute()
        
        if(outcome=='time_out'):
            rospy.loginfo("completed turning 90 degrees and buoy not found ")
        elif outcome=='buoy_found':
            rospy.loginfo("Found buoy while reaching -45%")
            return 'buoy_found'
        
        
        
        
        cm.userdata.u_r=self.resources.pose[header.PoseEnum.roll]
        cm.userdata.u_d=self.resources.pose[self.tDepth]
        cm.userdata.u_p=self.resources.pose[header.PoseEnum.pitch]
        cm.userdata.u_y=90
        
        rospy.loginfo("turning 90 degrees right ")
        outcome=cm.execute()
        
        if(outcome=='time_out'):
            rospy.loginfo("completed turning 90 degrees and buoy not found ")
            return 'time_out'
        elif outcome=='buoy_found':
            rospy.loginfo("Found buoy while reaching 90 degrees")
            return 'buoy_found'




class AligningVehicle(smach.State):
    def __init__(self,resources):
        """
        This state for just aligning the vehicle as done evrtime using feedback
        """
        smach.State.__init__(self, outcomes=['aligned','time_out'],input_keys=['time_out'])
        self.resources=resources
    def execute(self, ud):
        #comment this at last
        #self.resources=Interaction()
        #-------------
        
        
        rospy.loginfo("waiting for buoy_detect Server")
        buoyClient=SimpleActionClient(header.BUOY_DETECT_ACTION_SERVER, buoyAction)
        buoyClient.wait_for_server()
        rospy.loginfo("connected to server")
        
        goal=buoyGoal()
        goal.order=ip_header.ALLIGN_BUOY
        
        buoyClient.send_goal(goal, feedback_cb= self.feedback_cb)
        result=buoyClient.wait_for_result(rospy.Duration(ud.time_out))
        if not result:
            buoyClient.cancel_goal()
        
        if buoyClient.get_state()==GoalStatus.SUCCEEDED:
            rospy.loginfo("successfully aligned the vehicle with the gate ")
            return 'aligned'
        else:
            rospy.logerr("buoy not allogned and timed out before alliging it!!!")
            return 'timed_out'
        
    def feedback_cb(self,feed):
         
        ##publish this error data to the sensor and move the vehicle
        
        e=ipControllererror()
        e.dy=-feed.errorx
        e.dz=feed.errory
        
        self.resources.ipControllerPublisher.publish(e)
        
        ##done moving
        
        
    
        
        
            
       