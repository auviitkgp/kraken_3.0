
import roslib;roslib.load_manifest('mission_planner')
import rospy, actionlib
import smach
import kraken_msgs
from kraken_msgs.msg._controllerAction import controllerAction
from kraken_msgs.msg._controllerGoal import controllerGoal
from actionlib.simple_action_client import SimpleActionClient
from kraken_msgs.msg._advancedControllerAction import advancedControllerAction
from kraken_msgs.msg._krakenPose import krakenPose
from kraken_msgs.msg._ipControllererror import ipControllererror
from kraken_msgs.srv._moveAlongLine import moveAlongLine
from kraken_msgs.srv._krakenResetPose import krakenResetPose
from premap.srv._getLocation import getLocation
from resources import topicHeader
import geometry_msgs
from geometry_msgs.msg._Pose import Pose
from kraken_msgs.msg._advancedControllerActionGoal import advancedControllerActionGoal
from kraken_msgs.msg._advancedControllerGoal import advancedControllerGoal

class KrakenState(object):
    px=0 
    py=1
    pz=2 
    vx=3
    vy=4
    vz=5    
    ax=6
    ay=7
    az=8
    roll=9
    pitch=10
    yaw=11
    wroll=12
    wpitch=13
    wyaw=14
    def __init__(self):
        self.data=[None]*15
        



class Interaction(object):
    """
    this is the entry point for interaction with outside resources to the state
    machine. it contains all the service proxies and servers, meant to be passed
    to all the states. gets rid of global variables.
    the boolean values are checked in states
    """
    def __init__(self):
        self.heading = None

        """
        Setting default values
        """
        self.depth = None
        self.forward = None
        

        rospy.loginfo("Initializing mission planner interaction module...")
        
        

        rospy.Subscriber(name=topicHeader.NAV_POSE_ESTIMATED,data_class= krakenPose , callback=self.positionCallback, queue_size=100)
        
        
        self.setPointController=SimpleActionClient(topicHeader.CONTROL_SETPOINT_ACTION, controllerAction)
        rospy.loginfo("waiting for setPoint controller action Server")
        self.setPointController.wait_for_server()
        rospy.loginfo("Got controller server ")
    
        
        self.advancedControllerClient=SimpleActionClient(topicHeader.CONTROL_ADVANCEDCONTROLLER_ACTION, advancedControllerAction)
        rospy.loginfo("Waiting for advancedController action server")
        self.advancedControllerClient.wait_for_server()
        rospy.loginfo("Got advanced Controller Action Server ..")
        

        
        self.ipControllerPublisher=rospy.Publisher(name=topicHeader.CONTROL_IP_ERROR,
                                           data_class=ipControllererror, queue_size=100)
        
        
        self.moveAlongService=rospy.ServiceProxy(name=topicHeader.CONTROL_MOVEALONG_SERV,
                                                 service_class=moveAlongLine)
        rospy.loginfo("waiting for MoveAlongLine Service")
        self.moveAlongService.wait_for_service()
        rospy.loginfo("Got move along line service !!")
        
        
        self.resetPoseService=rospy.ServiceProxy(name=topicHeader.RESET_POSITION_SERVICE, service_class=krakenResetPose)
        rospy.loginfo("waiting for Reset Position Service")
        self.resetPoseService.wait_for_service()
        rospy.loginfo("Got move reser pose service !!")
        
        self.premapMarkerLocationService=rospy.ServiceProxy(name=topicHeader.PREMAP_LOCATION_SERVICE, service_class=getLocation)
        rospy.loginfo("waiting for premap location Service")
        self.premapMarkerLocationService.wait_for_service()
        rospy.loginfo("Got move premap location  service !!")
        
        #also do on and off camera services--To be implemented
        
        
        ############--------------------------

        rospy.loginfo("succesfully got all publishers and subsrcibers to mission planner !! ")
    
    def positionCallback(self,msg):
#        self.pose.x=msg.data
#        self.pose.header=msg.header
         self.state=KrakenState()
         self.state.data=msg.data
         
        
        












def main():
#     sm=smach.StateMachine(outcomes=['failed'])
#     sm.userdata.x=None;
#     with sm:
#         sm.add('INIT', initialState(), transitions={'initalized':'failed','failed':'fialed'});
    rospy.init_node(name="interaction", anonymous=False)
    cin=Interaction()
    goal2=advancedControllerGoal()
    goal2.flag=1
    goal2.x=13
    goal2.y=20
    cin.advancedControllerClient.send_goal_and_wait(goal2)
    rospy.spin()
    
        
    
if __name__ == '__main__':
    main()
         