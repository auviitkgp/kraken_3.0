#! /usr/bin/env python

import roslib
PKG = 'setyaw'
roslib.load_manifest(PKG)

import sys
import numpy as np
import time
from math import *

import actionlib
import std_msgs.msg
import rospy
from resources import topicHeader
# tools to get verbose
from resources import tools

from kraken_msgs.msg import thrusterData6Thruster
from kraken_msgs.msg import thrusterData4Thruster
from kraken_msgs.msg import absoluteRPY

from kraken_msgs.msg import setYawAction as actionMessageSetYawAction
from kraken_msgs.msg import setYawFeedback
from kraken_msgs.msg import setYawResult
from kraken_msgs.msg import setYawGoal

#from all_controllers.pid import PID
# from all_controllers.fuzzy import Fuzzy
# from all_controllers import fuzzyParams as Fparam

# Global variables
base_yaw = 0.0
FIRST_ITERATION = True
prevError = 0.0
Current_yaw = -1
Client_goal = None

import numpy as np

class PID():

	def __init__(self):

		self.k = [0,0,0]
		self.target = 0
		self.c_data = 0
		self.error = 0
		self.delta_e = 0
	def run(self):
		print self.error
		return 10*self.error+0.1*self.delta_e

	def output(self):
		print self.output


class SetYaw(object):
    """ Creates messages that are used to publish feedback/result to client
        Messages :
          *  _feedback :
                1. Current value of yaw
                2. Desired_yaw
                3. Error
          *  _result  :
                1. elapsed_time
          *  YawFeedback :
                - Same as _feedback
          *  thruster4Data :
                - thrusterData4Thruster
          *  thruster6Data :
                - thrusterData6Thruster
        Publishers :
          *  pub_thrusters4 : publish as 4 thruster model on thruster converter
          *  pub_thrusters4 : publish as 6 thruster model on thruster converter
          *  pub_yawFeedback : Publish on 'YawFeedback' topic for plotting feedback
          *  sub_ABSrpy : Subscribe absolute rpy from absoluteRPY publisher

    """

    # Initializing the messages to be used
    _feedback = setYawFeedback()
    _result = setYawResult()
    YawFeedback = setYawFeedback()
    thruster4Data=thrusterData4Thruster()
    thruster6Data=thrusterData6Thruster()

    def __init__(self, name):

        self._action_name = name
        self._as = actionlib.SimpleActionServer(
            self._action_name, actionMessageSetYawAction, execute_cb=self.execute_cb, auto_start=False)
        self._as.start()

        # Declare all Publisher and Subscribers here :
        self.pub_thrusters4 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER4, thrusterData4Thruster, queue_size = 2)
        self.pub_thrusters6 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER6, thrusterData6Thruster, queue_size = 2)
        self.pub_yawFeedback = rospy.Publisher('YawFeedback',setYawFeedback, queue_size = 2)
        self.sub_ABSrpy = rospy.Subscriber(topicHeader.ABSOLUTE_RPY, absoluteRPY, self.imuCB)

        rospy.loginfo('Server has started. Waiting for a goal.')


    def imuCB(self, dataIn):
        """
        This is imu call back function.

        1. Updates the Current value of yaw - Current_yaw
        2. Calculates error and delta_error based on 4 quadrant tangent = arctan2()
        3. Updates feedback to be sent to the client
            - Desired_yaw
            - Current_yaw
            - Error
        4. Update the data to be published on all the topics and publish contoller output and feedback to be plotted
        5. Debug messages
        """
        global Current_yaw
        global base_yaw
        global prevError
        global Client_goal
        global FIRST_ITERATION

        Current_yaw = dataIn.yaw

        # Get the updated base_yaw, when a new goal is recieved
        if  FIRST_ITERATION :
    		base_yaw = Current_yaw
    		FIRST_ITERATION = False

        # Goal flag to restrict, publishing the data only when goal is available
        rospy.logdebug("Goal flag : %s",Client_goal != None)

        if Client_goal != None :


            error = (base_yaw + Client_goal.yaw - Current_yaw)* 3.14 / 180
            YAW.error = np.arctan2(sin(error),cos(error))*180/3.14
            YAW.delta_error = YAW.error - prevError
	    print "Yaw.error : ",YAW.error,"prevError : ",prevError, "Delta_error : ",YAW.delta_error

            # Update the feedbacks to be sent
            self._feedback.Desired_yaw = (Client_goal.yaw + base_yaw)%360
            self._feedback.Current_yaw = Current_yaw
            self._feedback.Error = YAW.error

            # Update the feedbacks to published and plotted
            self.YawFeedback.Desired_yaw = self._feedback.Desired_yaw
            self.YawFeedback.Current_yaw = self._feedback.Current_yaw
            self.YawFeedback.Error = self._feedback.Error
            self.YawFeedback.header = std_msgs.msg.Header()
            self.YawFeedback.header.stamp = rospy.Time.now()

            # calculate the controller output from fuzzy control
            ControlOutput = YAW.run()
	    prevError = YAW.error

            # 6 Thruster model
            self.thruster6Data.data[0] = 0.0
            self.thruster6Data.data[1] = 0.0
            self.thruster6Data.data[2] = ControlOutput  # Left Thruster
            self.thruster6Data.data[3] = -1 * ControlOutput # Rigt Thruster
            self.thruster6Data.data[4] = 0.0
            self.thruster6Data.data[5] = 0.0
            self.thruster6Data.header  = std_msgs.msg.Header()
            self.thruster6Data.header.stamp = rospy.Time.now()

            # 4 Thruster model
            self.thruster4Data.data[0] = self.thruster6Data.data[0]
            self.thruster4Data.data[1] = self.thruster6Data.data[1]
            self.thruster4Data.data[2] = self.thruster6Data.data[4]
            self.thruster4Data.data[3] = self.thruster6Data.data[5]
            self.thruster4Data.header  = std_msgs.msg.Header()
            self.thruster4Data.header.stamp = rospy.Time.now()

            # Debug messages
            rospy.logdebug("--------")
            #rospy.logdebug("Current Yaw : %s",round(Current_yaw,3))
            #rospy.logdebug("Desired_yaw : %s",round(self._feedback.Desired_yaw,3))
            rospy.logdebug("Error : %s",round(YAW.error,3))
            rospy.logdebug("Delta_error : %s",round(YAW.delta_error ,3))
            #rospy.logdebug("Relative_Goal : %s",round(Client_goal.yaw,3))
            rospy.logdebug("Thruster data L : %s",self.thruster6Data.data[4])
            #rospy.logdebug("Thruster data R : %s",self.thruster6Data.data[5])



    def execute_cb(self, goal):
        """
        1. Update the goal each time client sends a new goal
        2. Wait for first imu data to be recieved, to calculate the error.
        3. calculate the thrust from fuzzy control and send to the thruster converter.
        4. publishes feedback back to the client
            - Desired_yaw
            - Current_yaw
            - Error
        """

        global Client_goal
        global FIRST_ITERATION
        global Current_yaw

        # Update the goal
        Client_goal = goal
        FIRST_ITERATION = True
        Current_yaw = -1

        r = rospy.Rate(10)

        # wait to receive starting value of yaw
        while(Current_yaw == -1):
            r.sleep()

        rospy.loginfo('I was asked to change yaw by %f', goal.yaw)
        initial_time = time.time()
        rospy.loginfo('Started setting yaw at %f', initial_time)
        success = True

        rospy.loginfo('YAW.error : %s',abs(YAW.error) > 0.5)
        while abs(YAW.error) > 0.5 :
            # check that preempt has not been requested by the client
            if self._as.is_preempt_requested():
                rospy.loginfo('%s: Preempted' % self._action_name)
                self._as.set_preempted()
                success = False

                # Set the thrust values to 0, to stop the thrusters
                self.thruster6Data.data = [0,0,0,0,0,0]
                self.thruster4Data.data = [0,0,0,0]
                self.pub_thrusters6.publish(self.thruster6Data)
                self.pub_thrusters4.publish(self.thruster4Data)

                break

            # publish the feedback
            self._as.publish_feedback(self._feedback)

            # publish controller output values to thruster converter
            self.pub_thrusters4.publish(self.thruster4Data)
            self.pub_thrusters6.publish(self.thruster6Data)
            self.pub_yawFeedback.publish(self.YawFeedback)
            r.sleep()

        if success :
            self._result.elapsed_time = time.time() - initial_time
            rospy.loginfo('%s: Succeeded' % self._action_name)
            self._as.set_succeeded(self._result)

if __name__ == '__main__':
	"""
	 1. Declare YAW as a Fuzzy object and Declare it's membership function and it's Range.
     2. Calls the class setyaw constructor
	"""
	# YAW = Fuzzy(Fparam.mf_types, Fparam.f_ssets)
	# YAW.io_ranges = Fparam.io_ranges
	YAW = PID()
	rospy.init_node('setyaw_action_server', log_level=(rospy.DEBUG if tools.getVerboseTag(sys.argv) else rospy.INFO))
	SetYaw('setYaw')
	rospy.spin()
