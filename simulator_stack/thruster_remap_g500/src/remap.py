#!/usr/bin/python
import roslib;roslib.load_manifest('thruster_remap_g500')

import rospy
import kraken_msgs
import geometry_msgs.msg as gs
from kraken_msgs.msg._forceData6Thruster import forceData6Thruster
from kraken_msgs.msg._forceData4Thruster import forceData4Thruster

from std_msgs.msg import Float64MultiArray
from resources import topicHeader
from std_srvs.srv import Empty

import numpy as np
from kraken_msgs.msg import imuData
from tf.transformations import euler_from_quaternion
from sensor_msgs.msg import Imu

thrusters_topic="/g500/thrusters_input"
pose_topic ='/g500/pose'
pub = rospy.Publisher(thrusters_topic, Float64MultiArray, queue_size=10)
imu_pub= rospy.Publisher(topicHeader.SENSOR_IMU, imuData, queue_size=10)
pose_pub= rospy.Publisher(topicHeader.SIMULATOR_POSE, gs.Pose, queue_size=10)

rospy.wait_for_service('/dynamics/reset')
reset=rospy.ServiceProxy('/dynamics/reset', Empty)

def remapandpublish(data):
	new_thrusters=[0]*5

	msg = Float64MultiArray()
	new_thrusters[0]=data.data[1]
	new_thrusters[1]=data.data[0]
	new_thrusters[2]=-data.data[5]
	new_thrusters[3]=-data.data[4]
	new_thrusters[4]=-data.data[2]-data.data[3]

	msg.data=new_thrusters
	pub.publish(msg)

def remapandpublish4(data):
	new_thrusters=[0]*5

	msg = Float64MultiArray()
	new_thrusters[0]=data.data[1]
	new_thrusters[1]=data.data[0]
	new_thrusters[4]=-data.data[2]-data.data[3]

	msg.data=new_thrusters
	pub.publish(msg)

def remapImuAndPublish(dataIn):
	'''
	Converts the message from sensors_msgs/Imu to kraken_msgs/imuData
	'''

	orientation = dataIn.orientation
	rpy = np.array(euler_from_quaternion([orientation.x, orientation.y, orientation.z, orientation.w])) * 180 / np.pi

	newImuData = imuData()
	newImuData.data = [0] * 13
	newImuData.data[0] = rpy[0]
	newImuData.data[1] = rpy[1]
	newImuData.data[2] = rpy[2]

	imu_pub.publish(newImuData)

def publish_pose(data):
	pose_pub.publish(data)


rospy.init_node('remapper', anonymous=False)

rospy.Subscriber('/g500/imu', Imu, remapImuAndPublish)
rospy.Subscriber(topicHeader.SIMULATOR_MODEL_FORCE_DATA_6_THRUSTERS, forceData6Thruster, remapandpublish)
rospy.Subscriber(topicHeader.SIMULATOR_MODEL_FORCE_DATA_4_THRUSTERS, forceData4Thruster, remapandpublish4)
rospy.Subscriber(pose_topic,gs.Pose,publish_pose)



rospy.spin()
