'''
Publishing the absolute Roll, Pitch and Yaw

- Axes :-

  - x -> Along the vehicle, pointing from the backplate to the front plate.
  - y -> Perpendicular to the vehicle; Points from the left surge thruster, 
         towards the right surge thruster.
  - z -> Normal to the vehicle; From the top of the hull, towards the DVL;

- Angle Convention :-

  - Angles in the xy plane are seen from the z-axis. (Same for other two planes.)
  - Angles are NEVER seen from the origin
  - Anti-clockwise angles are positive.

  - Refer to Euler Angles, on wikipedia
  - Reference: http://en.wikipedia.org/wiki/File:Yaw_Axis_Corrected.svg
'''

import roslib;roslib.load_manifest('absolute_rpy_publisher')
import rospy
import rospy
import time

from resources import topicHeader

import kraken_msgs
import kraken_msgs
from kraken_msgs.msg._imuData import imuData
from kraken_msgs.msg._absoluteRPY import absoluteRPY

# print topicHeader.SENSOR_IMU
# print topicHeader.ABSOLUTE_RPY

def imuCallback(imu):

	global absolute_rpy_publisher

	roll  = imu.data[0]
	pitch = imu.data[1]
	yaw   = imu.data[2]

	# Fix the roll, pitch by subtracting it from 360

	roll  = 360 - roll
	pitch = 360 - pitch

	abrpy       = absoluteRPY()
	abrpy.roll  = roll
	abrpy.pitch = pitch
	abrpy.yaw   = yaw

	absolute_rpy_publisher.publish(abrpy)

	# Store this in a message and publish it

absolute_rpy_publisher = rospy.Publisher(name=topicHeader.ABSOLUTE_RPY, data_class=absoluteRPY, queue_size=10)

rospy.Subscriber(name=topicHeader.SENSOR_IMU, data_class=imuData, callback=imuCallback)

rospy.init_node('absolute_roll_pitch_yaw_publisher')

rospy.spin()
