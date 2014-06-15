import roslib;roslib.load_manifest('mission_planner')
import rospy
import smach
import smach_ros
import header
from smach_ros.simple_action_state import SimpleActionState
from kraken_msgs.msg._advancedControllerAction import advancedControllerAction
from kraken_msgs.msg._advancedControllerGoal import advancedControllerGoal
from ip_msgs.msg._markerAction import markerAction
from ip_msgs.msg._markerGoal import markerGoal
from actionlib.simple_action_client import SimpleActionClient, SimpleGoalState
from kraken_msgs.msg._controllerAction import controllerAction
from kraken_msgs.msg._controllerGoal import controllerGoal
from ip_msgs.msg._markerFeedback import markerFeedback
from rospy.exceptions import ROSInitException
from ip_msgs.msg._markerResult import markerResult
from rospy.timer import Rate
from actionlib.simple_action_server import SimpleActionServer
import ip_header
from ip_msgs import *
from kraken_msgs.srv import *
from geometry_msgs.msg._Pose import Pose
from premap.srv._getLocation import getLocation, getLocationRequest,\
	getLocationResponse
import actionlib
from main import Interaction
from kraken_msgs.msg._krakenPose import krakenPose
from header import PoseEnum
from actionlib_msgs.msg._GoalStatus import GoalStatus
from kraken_msgs.msg._ipControllererror import ipControllererror
from math import tan


class SearchingMarker(smach.State):
	def __init__(self,resources):
		smach.State.__init__(self, outcomes=['found','time_out'])
		self.resources=resources
	def execute(self, ud):



		#do necessary stuff for moving in some path
		client=self.resources.advancedControllerClient
		goal= advancedControllerGoal()
		goal.depth=self.resources.depth
		goal.x=self.resources.pose[header.PoseEnum.x]
		goal.y=-0.1
		client.send_goal_and_wait(goal)
		#do necessary stuff for moving in some path
		return 'found'
	
	
class MovingVehicle(smach.State):
	def __init__(self,resources):
		smach.State.__init__(self, outcomes=['time_out','reached'],
							input_keys=['x','y','depth','time_out'])
		self.resources=resources
	def execute(self, ud):
		#comment this at last
		#self.resources=Interaction()
		#-------------
		
		goal=advancedControllerGoal()
		goal.flag=0;
		goal.x=ud.x
		goal.y=ud.y
		goal.depth=ud.depth
		
		self.resources.advancedControllerClient.cancel_all_goals();
		self.resources.advancedControllerClient.send_goal(goal)
		rospy.loginfo("moving to next marker at (%f, %f) and to depth %f",ud.x,ud.y,ud.depth)
		result=self.resources.advancedControllerClient.wait_for_result(rospy.Duration(20))
		
		print result
		
		if self.resources.advancedControllerClient.get_state()==GoalStatus.SUCCEEDED:
			rospy.loginfo("successfully moved to destination")
			return 'reached'
		else:
			rospy.logerr("timed out!! while moving to destination")
			return 'time_out'



class MarkerDetect(smach.State):
	def __init__(self,resources):
		"""
		This and Moving vehicle will run in concurrence container  
		"""
		smach.State.__init__(self, outcomes=['marker_found'],output_keys=['e_x','e_y'])
		self.resources=resources
	def execute(self, ud):
		#comment this at last
		#self.resources=Interaction()
		#-------------
		ipClient=SimpleActionClient(header.MARKER_DETECT_ACTION_SERVER, markerAction)
		rospy.loginfo("Waiting for IP marker action server")
		ipClient.wait_for_server()
		goal=markerGoal()
		goal.order=ip_header.DETECT_MARKER
		ipClient.cancel_all_goals()
		ipClient.send_goal(goal, done_cb=self.doneCb)
		result=ipClient.wait_for_result()
		if ipClient.get_state()==GoalStatus.SUCCEEDED:
			rospy.loginfo("succesfully detected marker :)")
			return 'marker_found'
		
		
	def doneCb(self,result):
		#implement this actully get this error so that u can set the approximate location of object and then use getLocation service to call
		
		#premap service
		self.resources.premapMarkerLocationService()
		




class AligningVehicle(smach.State):
	def __init__(self,resources):
		smach.State.__init__(self, outcomes=['aligned','timed_out'],input_keys=['time_out','e_x','e_y'])
		self.resources=resources
	def execute(self, ud):
		self.resources=Interaction()
		#do this job publish data to sensor controller
		rospy.loginfo("marker detected .....setting vehicle point to marker calling %s",header.RESET_POSE_SERVICE)
		resetClient=rospy.ServiceProxy(header.RESET_POSE_SERVICE, krakenResetPose)
		msg=krakenResetPoseRequest()
		
		resp=resetClient(msg)
		##help look here --- http://wiki.ros.org/rospy_tutorials/Tutorials/WritingServiceClient
		
		##################--within this it is done
		
		
		rospy.loginfo("aligning vehicle to set the marker")
		
		
		self.ipClient=SimpleActionClient(header.MARKER_DETECT_ACTION_SERVER,markerAction)
		self.ipClient.cancel_all_goals()
		
		goal=markerGoal()
		goal.order=header.ALLIGN_MARKER
		
		self.ipClient.send_goal(goal,done_cb= self.done_cb, feedback_cb= self.feedback_cb)
		self.ipClient.wait_for_result(rospy.Duration(ud.time_out))
		
		
		if self.result:
			rospy.loginfo("successfully aligned the vehicle with the marker ")
			#again use the service to reset the vehicle's position 
			msg=krakenResetPoseRequest()
			resp=resetClient(msg)
			########################
			
			self.ipClient.cancel_goal()
			return 'aligned'
		else:
			self.ipClient.cancel_goal()
			rospy.logerr("marker unaligned and timed out!!!")
			return 'timed_out'
		
	def feedback_cb(self,feed):
		##publish this error data to the sensor and move the vehicle
		msg=ipControllererror()
		msg.dy=feed.errorx
		msg.dx=msg.dy/tan(feed.errorangle)
		self.resources.ipControllerPublisher.publish(msg)
		
		##done moving
		
		
	def done_cb(self,result):
		if result.sequence==header.MARKER_ALLIGNED:
			self.result=True
		else:
			self.result=False
				
	
		
		
	


class NextTaskMarker(smach.State):
	
	def __init__(self):
		smach.State.__init__(self, 
							outcomes=['succeeded','failed','marker_undetected','marker_unalligned'],
							input_keys=['x2','y2','depth'])
		rospy.init_node("nextTaskNode")
	def execute(self, ud):
		rospy.loginfo("Starting next task....")
		cm_movingToPoint=smach.Concurrence(outcomes=['marker_detected','marker_undetected','time_out'],
										 default_outcome='marker_undetected', 
										 outcome_map={'marker_detected':{
																		'MARKER_DETECT':'succeeded'
																		},
														'time_out':{
																	'CONTROLLER':'succeeded'
																	}
													})
		moveGoal=advancedControllerGoal()
		moveGoal.x=ud.x2
		moveGoal.y=ud.y2
		moveGoal.depth=ud.depth
		markGoal=markerGoal()
		markGoal.order=header.DETECT_MARKER
		with cm_movingToPoint:
			smach.Concurrence.add('CONTROLLER', 
								SimpleActionState("advancedController",
												advancedControllerAction , 
												goal=moveGoal))
			smach.Concurrence.add('MARKER_DETECT', 
								SimpleActionState("marker_detect",
												goal=markGoal))
		
		
		return 'done';

	
	
def main():
	
	rospy.init_node("next_Task_Node")
	sm_nextTaskMarker=smach.StateMachine(outcomes=['succeeded','failed','marker_undetected','marker_unalligned'])
	
	
	#initialize these parameters
	resources=Interaction()
	premapClient=resources.premapMarkerLocationService
	req=getLocationRequest()
	req.id=1#change the id as required
	res=premapClient(req)
	
	sm_nextTaskMarker.userdata.x=res.x
	sm_nextTaskMarker.userdata.y=res.y
	sm_nextTaskMarker.userdata.depth=10##may be u've to send this from premap xml file itself
	
	with sm_nextTaskMarker:
		cm=smach.Concurrence(outcomes=['marker_detected','vehicle_reached','marker_undetected'],
							 default_outcome='marker_undetected')
		cm.userdata.x=res.x
		cm.userdata.y=res.y	
		cm.userdata.depth=10
		cm.userdata.time=10
		cm.userdata.ex=None
		cm.userdata.ey=None
		with cm:
			smach.Concurrence.add('MOVING_STATE',
								 MovingVehicle(resources),
								 remapping={'x':'x','y':'y','depth':'depth','time_out':'time'})
			smach.Concurrence.add('DETECTING_STATE',MarkerDetect(resources),
									remapping={'e_x':'ex','e_y':'ey'})
		sis = smach_ros.IntrospectionServer('server_name', cm, '/SM_CM')
		sis.start()
		outcome=cm.execute()
		
		smach.StateMachine.add('CONCURRENCE_MACHINE', 
								cm,
								transitions={'marker_detected':'ALIGINING_VEHICLE','vehicle_reached':'SEARCHING_MARKER'})
		smach.StateMachine.add('SEARCHING_MARKER',
								SearchingMarker(resources),
								transitions={'found':'ALIGINING_VEHICLE','time_out':'marker_undetected'})
		
		smach.StateMachine.add('ALIGINING_VEHICLE',
								AligningVehicle(resources),
								transitions={'aligned':'succeeded','timed_out':'marker_unalligned'}, 
								remapping={'time_out':'time','e_x':'ex','e_y':'ey'}
								)
			
	sis = smach_ros.IntrospectionServer('server_name', sm_nextTaskMarker, '/SM_ROOT')
	sis.start()
	
	outcome=sm_nextTaskMarker.execute()
	rospy.spin()
	sis.stop()
#	if outcome=='succeeded':
		
		
if __name__=='__main__':
	main();
	