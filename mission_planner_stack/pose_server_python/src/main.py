import roslib;roslib.load_manifest('mission_planner')
import rospy
import rospy
from resources import topicHeader

import kraken_msgs
import kraken_msgs
from kraken_msgs.msg._imuData import imuData
from kraken_msgs.msg._dvlData import dvlData

from kalman_estimator import *

dt = 0.1
NUM_VARIABLE_IN_STATE = 4
INDEX_VEL_X = 3
INDEX_VEL_Y = 4

# state = [position-x, position-y, velocity-x, velocity-y]
state = matrix([[0.0], [0.0], [0.], [0.]]) # initial state (location and velocity)
statefilled = 2
measurements = [[0.0, 0.0]]
P = matrix([[1000., 0., 0., 0.], [0., 1000., 0., 0.], [0., 0., 0, 0.], [0., 0., 0., 0]])# initial uncertainty

def imuCallback(imu):
	print "Entered IMU callback!"

def dvlCallback(dvl):
	global state
	global measurements
	global statefilled

	print "Entered DVL callback!"

	## Extract from the variable dvl

	vx = int(raw_input("Enter vx: "))
	vy = int(raw_input("Enter vy: "))
	
	## End extract step

	this_iteration_measurement = [vx, vy]

	state.setvalue(INDEX_VEL_X, 1, vx)
	state.setvalue(INDEX_VEL_Y, 1, vy)

	statefilled += 2	
	measurements.append(this_iteration_measurement)

# subscribe to IMU and DVL

# extract pitch and yaw from IMU
# extract vx and vy from DVL
# take positions state and y from previous state. (Take (state, y) = (0, 0) initially.)

imu_topic_name = topicHeader.SENSOR_IMU
dvl_topic_name = topicHeader.SENSOR_DVL

print imu_topic_name
print dvl_topic_name

rospy.init_node('pose_server_python_node', anonymous=True)

rospy.Subscriber(name=imu_topic_name, data_class=imuData, callback=imuCallback)
rospy.Subscriber(name=dvl_topic_name, data_class=dvlData, callback=dvlCallback)

while(1):

	# if all the data has been accumulated in the state variable

	if(statefilled >= NUM_VARIABLE_IN_STATE):

		(new_state, new_P) = kalman_estimate(state, P, measurements[-1])

		state.setvalue(1, 1, new_state.getvalue(1, 1))
		state.setvalue(2, 1, new_state.getvalue(2, 1))

		statefilled = 2

		print "new state: "
		new_state.show()
		print "new P matrix: "
		new_P.show()

rospy.spin()
