
import roslib;roslib.load_manifest('mission_planner')
import rospy, actionlib
import smach
import kraken_msgs
from kraken_msgs.msg._controllerAction import controllerAction
from kraken_msgs.msg._controllerGoal import controllerGoal
from actionlib.simple_action_client import SimpleActionClient
import header
from kraken_msgs.msg._advancedControllerAction import advancedControllerAction
from kraken_msgs.msg._krakenPose import krakenPose
from kraken_msgs.msg._ipControllererror import ipControllererror
from kraken_msgs.srv._moveAlongLine import moveAlongLine
from kraken_msgs.srv._krakenResetPose import krakenResetPose
from premap.srv._getLocation import getLocation
from jinja2._stringdefs import No


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
        
        

        rospy.Subscriber(name=header.ESTIMATED_POSE_TOPIC_NAME,data_class= krakenPose , callback=self.positionCallback, queue_size=100)
        
        
        self.setPointController=SimpleActionClient(header.CONTROLLER_SERVER, controllerAction)
        rospy.loginfo("waiting for setPoint controller action Server")
        self.setPointController.wait_for_server()
        rospy.loginfo("Got controller server ")
    
        
        self.advancedControllerClient=SimpleActionClient(header.ADVANCED_CONTROLLER_SERVER, advancedControllerAction)
        rospy.loginfo("Waiting for advancedController action server")
        self.advancedControllerClient.wait_for_server()
        rospy.loginfo("Got advanced Controller Action Server ..")
        

        
        self.ipControllerPublisher=rospy.Publisher(name=header.IP_CONTROLLER_PUBLISHING_TOPIC,
                                           data_class=ipControllererror, queue_size=100)
        
        
        self.moveAlongService=rospy.ServiceProxy(name=header.MOVE_ALONG_SERVICE_NAME,
                                                 service_class=moveAlongLine)
        rospy.loginfo("waiting for MoveAlongLine Service")
        self.moveAlongService.wait_for_service()
        rospy.loginfo("Got move along line service !!")
        
        
        self.resetPoseService=rospy.ServiceProxy(name=header.RESET_POSE_SERVICE, service_class=krakenResetPose)
        rospy.loginfo("waiting for Reset Position Service")
        self.resetPoseService.wait_for_service()
        rospy.loginfo("Got move reser pose service !!")
        
        self.premapMarkerLocationService=rospy.ServiceProxy(name=header.PREMAP_LOCATION_SERVICE, service_class=getLocation)
        rospy.loginfo("waiting for premap location Service")
        self.premapMarkerLocationService.wait_for_service()
        rospy.loginfo("Got move premap location  service !!")
        
        #also do on and off camera services--To be implemented
        
        
        ############--------------------------

        rospy.loginfo("succesfully got all publishers and subsrcibers to mission planner !! ")
    
    def positionCallback(self,msg):
        self.pose=msg.data
        self.pose.header=msg.header













def main():
#     sm=smach.StateMachine(outcomes=['failed'])
#     sm.userdata.x=None;
#     with sm:
#         sm.add('INIT', initialState(), transitions={'initalized':'failed','failed':'fialed'});
    pass;
        
    
if __name__ == '__main__':
    main()
         