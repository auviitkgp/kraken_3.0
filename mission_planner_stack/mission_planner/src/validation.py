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
from ip_msgs.msg._vgateAction import vgateAction


class DetectingGate(smach.State):
    def __init__(self,resources):
        """
        This state for just detecting the gate  
        and intiialzing gate parameters and then searching gate
        """
        smach.State.__init__(self, outcomes=['gate_found','time_out'],input_keys=['depth','threshold'])
        self.resources=resources
    def execute(self, ud):
        #comment this at last
        self.resources=Interaction()
        #-------------
        
        
        rospy.loginfo("[TP]...Starting gate detecting task...")
        
        """
        This is a concurrence container containing the SimpleActionState for both controllerSevrer and buoyServer
        the outcome map specifices what to do when one of the states of cm reach a final outcome
        Everytime the controller goal is provided using input key of the state
        
        """
        cm=smach.Concurrence(outcomes=['gate_found','gate_unfound','time_out'], 
                            default_outcome='gate_unfound', 
                            outcome_map={
                                         'gate_found':{'DETECT_SERVER':'succeeded'},
                                         'time_out':{'CONTROLLER_SERVER':'succeeded'}
                                         },
                            input_keys=['u_r','u_p','u_y','u_d'])
        cm.userdata.u_r=None
        cm.userdata.u_p=None
        cm.userdata.u_y=None
        cm.userdata.u_d=None
        
        with cm:
            goalb=buoyGoal()
            goalb.order=ip_header.ALLIGN_VGATE
            smach.Concurrence.add('DETECT_SERVER', SimpleActionState(header.VALIDATION_GATE_ACTION_SERVER, 
                                                                 vgateAction, goal))
            
            smach.Concurrence.add('CONTROLLER_SERVER',
                                  SimpleActionState(header.CONTROLLER_SERVER,
                                                    controllerAction,
                                                    goal_slots=['r','p','y','d']),
                                  remapping={
                                             'r':'u_r','p':'u_p','y':'u_y','d':'u_d'
                                             }
                                  )
        
        
        
        """
        activating the gate and going to a specific depth
        """
        cm.userdata.u_r=self.resources.pose[header.PoseEnum.roll]
        cm.userdata.u_d=self.tDepth
        cm.userdata.u_p=self.resources.pose[header.PoseEnum.pitch]
        cm.userdata.u_y=self.resources.pose[header.PoseEnum.yaw]
        rospy.loginfo("moving to depth %f",self.tDepth)
        
        
        outcome=cm.execute()
        
        if(outcome=='time_out'):
            rospy.loginfo("succesfully reached depth %f ",self.tDepth)
        elif outcome=='gate_found':
            rospy.loginfo("Found gate while reaching depth %d !!",self.tDepth)
            return 'gate_found'
        
        
        
        """
           now setting yaw -45 degrees and checking for gate within this range  
        """
        cm.userdata.u_r=self.resources.pose[header.PoseEnum.roll]
        cm.userdata.u_d=self.resources.pose[self.tDepth]
        cm.userdata.u_p=self.resources.pose[header.PoseEnum.pitch]
        cm.userdata.u_y=-45
        rospy.loginfo("turning --45 degrees left ")
        outcome=cm.execute()
        
        if(outcome=='time_out'):
            rospy.loginfo("completed turning 90 degrees and gate not found ")
        elif outcome=='gate_found':
            rospy.loginfo("Found gate while reaching -45%")
            return 'gate_found'
        
        
        
        
        cm.userdata.u_r=self.resources.pose[header.PoseEnum.roll]
        cm.userdata.u_d=self.resources.pose[self.tDepth]
        cm.userdata.u_p=self.resources.pose[header.PoseEnum.pitch]
        cm.userdata.u_y=90
        
        rospy.loginfo("turning 90 degrees right ")
        outcome=cm.execute()
        
        if(outcome=='time_out'):
            rospy.loginfo("completed turning 90 degrees and gate not found ")
            return 'time_out'
        elif outcome=='gate_found':
            rospy.loginfo("Found gate while reaching 90 degrees")
            return 'gate_found'




class AligningVehicle(smach.State):
    def __init__(self,resources):
        """
        This state for just aligning the vehicle as done evrtime using feedback
        """
        smach.State.__init__(self, outcomes=['aligned','time_out'],input_keys=['time_out'])
        self.resources=resources
    def execute(self, ud):
        #comment this at last
        self.resources=Interaction()
        #-------------
        
        
        rospy.loginfo("waiting for gate_detect Server")
        gateClient=SimpleActionClient(header.VALIDATION_GATE_ACTION_SERVER, gateAction)
        gateClient.wait_for_server()
        rospy.loginfo("connected to server")
        
        goal=gateGoal()
        goal.order=ip_header.ALLIGN_VGATE
        
        gateClient.send_goal(goal, feedback_cb= self.feedback_cb)
        result=gateClient.wait_for_result(rospy.Duration(ud.time_out))
        if not result:
            gateClient.cancel_goal()
        
        if gateClient.get_state()==GoalStatus.SUCCEEDED:
            rospy.loginfo("successfully aligned the vehicle with the gate ")
            return 'aligned'
        else:
            rospy.logerr("gate not alligned and timed out before alliging it!!!")
            return 'timed_out'
        
    def feedback_cb(self,feed):
         
        ##publish this error data to the sensor and move the vehicle
        
        e=ipControllererror()
        e.dy=-feed.errorx
        e.dz=feed.errory
        
        self.resources.ipControllerPublisher.publish(e)
        
        ##done moving
        
        
    
        
        
            
       