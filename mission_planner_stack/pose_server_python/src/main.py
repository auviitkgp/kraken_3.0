import roslib;roslib.load_manifest('pose_server_python')
import rospy
import rospy
import time
from resources import topicHeader

import kraken_msgs
import kraken_msgs
from kraken_msgs.msg._absoluteRPY import absoluteRPY
from kraken_msgs.msg._dvlData import dvlData
from kraken_msgs.msg._positionData import positionData
from kraken_msgs.msg._stateData import stateData

from kalman_estimator import *

dt = 0.1
NUM_VARIABLE_IN_STATE = 4
INDEX_VEL_X = 3
INDEX_VEL_Y = 4
CONVERTED_TO_WORLD = False
FIRST_ITERATION = True
base_roll = 0
base_pitch = 0
base_yaw = 0

# state = [position-x, position-y, velocity-x, velocity-y]
state = matrix([[0.0], [0.0], [0.], [0.]]) # initial state (location and velocity)
statefilled = 2
measurements = [[0.0, 0.0]]
P = matrix([[1000., 0., 0., 0.], [0., 1000., 0., 0.], [0., 0., 0, 0.], [0., 0., 0., 0]])# initial uncertainty

oldtime = 0
def dvlCallback2(dvl):
	global oldtime
	t = dvl.data

	if oldtime == 0:

		oldtime = time.time()
		# print t[3], t[4]
		return

	if time.time() - oldtime >= 1:

		oldtime = time.time()
		# print t[3], t[4]

def transformCallback(abrpy):
	global statefilled
	global state
	global CONVERTED_TO_WORLD
	global FIRST_ITERATION
	global base_roll
	global base_pitch
	global base_yaw

	vx = state.getvalue(INDEX_VEL_X, 1)
	vy = state.getvalue(INDEX_VEL_Y, 1)

	# yaw, pitch, roll

	roll = abrpy.roll
	pitch = abrpy.pitch
	yaw = abrpy.yaw

	if FIRST_ITERATION:

		base_roll = roll
		base_pitch = pitch
		base_yaw = yaw
		FIRST_ITERATION = False

	print "IMU (Un-Corrected): ", 
	print round(roll, 2), 
	print round(pitch, 2), 
	print round(yaw, 2)

	roll = roll - base_roll
	pitch = pitch - base_pitch
	yaw = yaw - base_yaw

	print "IMU (Corrected): ", 
	print round(roll, 2), 
	print round(pitch, 2), 
	print round(yaw, 2)

	## Convert the roll, pitch and yaw to radians.

	yaw = yaw * 3.14 / 180
	roll = roll * 3.14 / 180
	pitch = pitch * 3.14 / 180

	## Refer: http://www.chrobotics.com/library/understanding-euler-angles

	bodytoworld = matrix(
		[
		[ # row 1
			cos(yaw) * cos(pitch),  
		  sin(yaw) * cos(pitch), 
		 	-1 * sin(pitch)
		],

		[ # row 2
			cos(yaw) * sin(pitch) * sin(roll) - sin(yaw) * cos(roll), 
		 	sin(yaw) * sin(pitch) * sin(roll) + cos(yaw) * cos(roll),
		 	cos(pitch) * sin(roll)
		],
		[ # row 3
			cos(yaw) * sin(pitch) * cos(roll) + sin(yaw) * sin(roll),
		 	sin(yaw) * sin(pitch) * cos(roll) - cos(yaw) * sin(roll),
		 	cos(pitch) * cos(roll)
		]
	  ]
	)

	bodytoworld = bodytoworld.transpose()

	# print bodytoworld

	# import numpy
	# print numpy.linalg.det(bodytoworld.value)

	# bodytoworld.show()

	vel_wrt_body = matrix([[vx],[vy],[0.]])

	vel_wrt_world = bodytoworld * vel_wrt_body

	vel_wrt_world.show()

	CONVERTED_TO_WORLD = True

	# print "Entered IMU callback!"

def dvlCallback(dvl):
	global state
	global measurements
	global statefilled

	# print dvl.data

	# print "Entered DVL callback!"

	## Extract from the variable dvl

	# vx = int(raw_input("Enter vx: "))
	# vy = int(raw_input("Enter vy: "))

	vx = dvl.data[3]
	vy = -1 * dvl.data[4]

	# print vx, vy

	roll = dvl.data[0]
	pitch = dvl.data[1]
	yaw = dvl.data[2]

	# print "DVL: ", roll, pitch, yaw
	## End extract step

	this_iteration_measurement = [vx, vy]

	state.setvalue(INDEX_VEL_X, 1, vx)
	state.setvalue(INDEX_VEL_Y, 1, vy)

	statefilled += 2	
	measurements.append(this_iteration_measurement)

def publishStateAndPosition(state_matrix):

	global position_publisher
	global state_publisher

	vx = state_matrix.getvalue(INDEX_VEL_X, 1)
	vy = state_matrix.getvalue(INDEX_VEL_Y, 1)
	pos_x = state_matrix.getvalue(1, 1)
	pos_y = state_matrix.getvalue(2, 1)

	present_position = positionData()
	present_state = stateData()

	present_position.x_coordinate = present_state.x_coordinate = pos_x
	present_position.y_coordinate = present_state.y_coordinate = pos_y

	present_state.velocity_x = vx
	present_state.velocity_y = vy

	# print present_state
	# print present_position

	position_publisher.publish(present_position)
	state_publisher.publish(present_state)

	return

# subscribe to IMU and DVL

# extract pitch and yaw from IMU
# extract vx and vy from DVL
# take positions state and y from previous state. (Take (state, y) = (0, 0) initially.)

absolute_rpy_topic_name = topicHeader.ABSOLUTE_RPY
dvl_topic_name = topicHeader.SENSOR_DVL
publish_state_topic_name = topicHeader.POSE_SERVER_STATE
publish_position_topic_name = topicHeader.PRESENT_POSE

# print absolute_rpy_topic_name
# print dvl_topic_name
# print publish_position_topic_name
# print publish_state_topic_name

rospy.init_node('pose_server_python_node', anonymous=True)

rospy.Subscriber(name=absolute_rpy_topic_name, data_class=absoluteRPY, callback=transformCallback)
rospy.Subscriber(name=dvl_topic_name, data_class=dvlData, callback=dvlCallback)

position_publisher = rospy.Publisher(publish_position_topic_name, positionData, queue_size=10)
state_publisher = rospy.Publisher(publish_state_topic_name, stateData, queue_size=10)

# publishStateAndPosition(state_matrix)

while(1):

	# if all the data has been accumulated in the state variable

	if(statefilled >= NUM_VARIABLE_IN_STATE and CONVERTED_TO_WORLD):

		(new_state, new_P) = kalman_estimate(state, P, measurements[-1])

		state.setvalue(1, 1, new_state.getvalue(1, 1))
		state.setvalue(2, 1, new_state.getvalue(2, 1))

		statefilled = 2
		CONVERTED_TO_WORLD = False

		publishStateAndPosition(state)

		# print "new state: "
		# new_state.show()
		# print "new P matrix: "
		# new_P.show()
		
		P = matrix(new_P.value)

rospy.spin()
