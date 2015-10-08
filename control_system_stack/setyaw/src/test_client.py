#! /usr/bin/env python

PKG = 'setyaw'
import roslib; roslib.load_manifest(PKG)
import rospy

# Brings in the SimpleActionClient
import actionlib

# Brings in the messages used by the fibonacci action, including the
# goal message and the result message.
from kraken_msgs.msg import setYawAction as actionMessageSetYawAction
from kraken_msgs.msg import setYawFeedback
from kraken_msgs.msg import setYawResult
from kraken_msgs.msg import setYawGoal

def serverSentSomeFeedback(feedback_obj):

    rospy.loginfo('Recieved feedback from the server: %f', feedback_obj.feedback_yaw)

def yawHasBeenSet(result_obj, goal):

    rospy.loginfo('Done! The server said that the yaw has been set!')

def set_yaw_client(yaw_parameter):

    rospy.loginfo('Entered client function')
    # Creates the SimpleActionClient, passing the type of the action
    # (FibonacciAction) to the constructor.
    client = actionlib.SimpleActionClient('setYaw', actionMessageSetYawAction)

    # Waits until the action server has started up and started
    # listening for goals.
    client.wait_for_server()

    # Creates a goal to send to the action server.
    goal = setYawGoal(yaw=yaw_parameter)

    # Sends the goal to the action server.
    client.send_goal(goal, done_cb = yawHasBeenSet, feedback_cb = serverSentSomeFeedback)

    # Waits for the server to finish performing the action.
    client.wait_for_result()

    # Prints out the result of executing the action
    return client.get_result()  # A FibonacciResult

if __name__ == '__main__':
    try:
        # Initializes a rospy node so that the SimpleActionClient can
        # publish and subscribe over ROS.
        rospy.init_node('fibonacci_client_py')
        wanted_yaw = float(raw_input('Enter the yaw you need: '))
        result = set_yaw_client(wanted_yaw)
        print "Elapsed time:", ', ', result.elapsed_time
    except rospy.ROSInterruptException:
        print "program interrupted before completion"
