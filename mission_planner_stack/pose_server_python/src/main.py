import roslib;roslib.load_manifest('mission_planner');
import rospy
import rospy
import time
from resources import topicHeader

import kraken_msgs
import kraken_msgs
from kraken_msgs.msg._absoluteRPY import absoluteRPY
from kraken_msgs.msg._dvlData import dvlData
from kraken_msgs.msg._imuData import imuData
from kraken_msgs.msg._positionData import positionData
from kraken_msgs.msg._stateData import stateData

from kalman_estimator import *

import time

DEBUG = False

dt = 0.1
NUM_VARIABLE_IN_STATE = 6
INDEX_VEL_X = 4
INDEX_VEL_Y = 5
INDEX_VEL_Z = 6
CONVERTED_TO_WORLD = False
ACC_MATRIX_POPULATED = False
FIRST_ITERATION = True
FIRST_ITERATION_VEL = True
base_roll = 0
base_pitch = 0
base_yaw = 0
prev_vel = [0., 0., 0.]
prev_time = time.time()

# state = [position-x, position-y, velocity-x, velocity-y]
state = matrix([[0.], [0.], [0.], [0.], [0.], [0.]]) # initial state (location and velocity)
statefilled = 3
measurements = [[0.0, 0.0, 0.]]
P = matrix([
            [1.5  , 0.   , 0.    , 0.  ,0.  ,0. ], 
            [0.   , 1.5  , 0.    , 0.  ,0.  ,0. ], 
            [0.   , 0.   , 1.5   , 0.  ,0.  ,0. ],
            [0.   , 0.   , 0.    , 0.2 ,0.  ,0. ],
            [0.   , 0.   , 0.    , 0.  ,0.2 ,0. ], 
            [0.   , 0.   , 0.    , 0.  ,0.  ,0.2]
           ])# initial uncertainty

u = matrix([[0.], [0.], [0.]])

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

	if DEBUG:

		print 'entered absolute rpy'

        vx = measurements[-1][0]
        vy = measurements[-1][1]
        vz = measurements[-1][2]

	# yaw, pitch, roll

	roll = abrpy.roll
	pitch = abrpy.pitch
	yaw = abrpy.yaw

	if FIRST_ITERATION:

		base_roll = roll
		base_pitch = pitch
		base_yaw = yaw
		FIRST_ITERATION = False

	# print "IMU (Un-Corrected): ", 
	# print round(roll, 2), 
	# print round(pitch, 2), 
	# print round(yaw, 2)

	roll = roll - base_roll
	pitch = pitch - base_pitch
	yaw = yaw - base_yaw

	# print "IMU (Corrected): ", 
	# print round(roll, 2), 
	# print round(pitch, 2), 
	# print round(yaw, 2)

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

	vel_wrt_body = matrix([[vx],[vy],[vz]])

	vel_wrt_world = bodytoworld * vel_wrt_body

	state.setvalue(INDEX_VEL_X, 1, vel_wrt_world.getvalue(1, 1))
	state.setvalue(INDEX_VEL_Y, 1, vel_wrt_world.getvalue(2, 1))
	state.setvalue(INDEX_VEL_Z, 1, vel_wrt_world.getvalue(3, 1))

	statefilled += 3

	# vel_wrt_world.show()

	CONVERTED_TO_WORLD = True

	# print "Entered IMU callback!"

def dvlCallback(dvl):
	global state
	global measurements
	global statefilled
	global FIRST_ITERATION_VEL
        global u
        global prev_time
        global ACC_MATRIX_POPULATED

	# print dvl.data

	if DEBUG:

		print "Entered DVL callback!"

	## Extract from the variable dvl

	# vx = int(raw_input("Enter vx: "))
	# vy = int(raw_input("Enter vy: "))

	vx = dvl.data[3]
	vy = -1 * dvl.data[4]
	vz = -1 * dvl.data[5]


        print "DVL: ", round(vx, 2), round(vy, 2), round(vz, 2)

        if DEBUG:

  	    print "DVL: vz: ", round(vz, 2)

	# print vx, vy, vz

	if FIRST_ITERATION_VEL:

		global prev_vel
		prev_vel = [vx, vy, vz]
		FIRST_ITERATION_VEL = False

        dt = time.time() - prev_time
        prev_time = time.time()

        ax = (vx - prev_vel[0]) / dt
        ay = (vy - prev_vel[1]) / dt
        az = (vz - prev_vel[2]) / dt

        u = matrix([[ax], [ay], [az]])

	# print "DVL: ", roll, pitch, yaw
	## End extract step

	this_iteration_measurement = [vx, vy, vz]

        ACC_MATRIX_POPULATED = True

	# print "States: ", statefilled
	measurements.append(this_iteration_measurement)

def imuCallback(imu):

	'''
	generates the u matrix that is given as an input to the Kalman estimator

	imu -> imuData, kraken_msgs
	'''

        return
	
	if DEBUG:

		print 'entered imu callback'

	global u
	global ACC_MATRIX_POPULATED

	ax = imu.data[3]
	ay = imu.data[4]
	az = imu.data[5]

	u = matrix([[ax], [ay], [az]])

	ACC_MATRIX_POPULATED = True

def publishStateAndPosition(state_matrix):

	global position_publisher
	global state_publisher

	if DEBUG:

		print "entered publish state-pos"

	vx = state_matrix.getvalue(INDEX_VEL_X, 1)
	vy = state_matrix.getvalue(INDEX_VEL_Y, 1)
        vz = state_matrix.getvalue(INDEX_VEL_Z, 1)
	pos_x = state_matrix.getvalue(1, 1)
	pos_y = state_matrix.getvalue(2, 1)
	pos_z = state_matrix.getvalue(3, 1)

        print round(pos_x, 2), round(pos_y, 2), round(pos_z, 2), round(vx, 2), round(vy, 2), round(vz, 2)

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
imu_topic_name = topicHeader.SENSOR_IMU
# print imu_topic_name
publish_state_topic_name = topicHeader.POSE_SERVER_STATE
publish_position_topic_name = topicHeader.PRESENT_POSE

# print absolute_rpy_topic_name
# print dvl_topic_name
# print publish_position_topic_name
# print publish_state_topic_name

rospy.init_node('pose_server_python_node', anonymous=True)

rospy.Subscriber(name=absolute_rpy_topic_name, data_class=absoluteRPY, callback=transformCallback)
rospy.Subscriber(name=dvl_topic_name, data_class=dvlData, callback=dvlCallback)
rospy.Subscriber(name=imu_topic_name, data_class=imuData, callback=imuCallback)

position_publisher = rospy.Publisher(publish_position_topic_name, positionData, queue_size=10)
state_publisher = rospy.Publisher(publish_state_topic_name, stateData, queue_size=10)

# publishStateAndPosition(state_matrix)

while(1):

	looprate = rospy.Rate(10)

	# if all the data has been accumulated in the state variable

	if(statefilled >= NUM_VARIABLE_IN_STATE and CONVERTED_TO_WORLD and ACC_MATRIX_POPULATED):

	# if True:

		(new_state, new_P) = kalman_estimate(state, P, measurements[-1], u)

		state.setvalue(1, 1, new_state.getvalue(1, 1))
		state.setvalue(2, 1, new_state.getvalue(2, 1))
		state.setvalue(3, 1, new_state.getvalue(3, 1))

		statefilled = 3
		CONVERTED_TO_WORLD = False
		ACC_MATRIX_POPULATED = False

		publishStateAndPosition(state)

		# print "new state: "
		# new_state.show()
		# print "new P matrix: "
		# new_P.show()
		
		P = matrix(new_P.value)

	looprate.sleep()

rospy.spin()
