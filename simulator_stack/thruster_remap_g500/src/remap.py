#!/usr/bin/python
import roslib;roslib.load_manifest('thruster_remap_g500')

import rospy
import kraken_msgs
from kraken_msgs.msg._forceData6Thruster import forceData6Thruster
from kraken_msgs.msg._forceData4Thruster import forceData4Thruster

from std_msgs.msg import Float64MultiArray 
from resources import topicHeader
from std_srvs.srv import Empty

thrusters_topic="/g500/thrusters_input"

pub = rospy.Publisher(thrusters_topic, Float64MultiArray,queue_size=10)

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




rospy.Subscriber(topicHeader.SIMULATOR_MODEL_FORCE_DATA_6_THRUSTERS, forceData6Thruster, remapandpublish)
rospy.Subscriber(topicHeader.SIMULATOR_MODEL_FORCE_DATA_4_THRUSTERS, forceData4Thruster, remapandpublish4)
rospy.init_node('remapper', anonymous=False)


rospy.spin()