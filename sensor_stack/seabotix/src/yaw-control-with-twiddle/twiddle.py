#!/usr/bin/env python

PKG = 'seabotix'

import roslib; roslib.load_manifest(PKG)
import serial
import rospy
import numpy
from kraken_msgs.msg import thrusterData6Thruster
from kraken_msgs.msg import thrusterData4Thruster
from kraken_msgs.msg import imuData
from kraken_msgs.msg import absoluteRPY
from resources import topicHeader

import numpy # for the function arctan2
from math import *

PKG = 'seabotix'

N = 100
errorP = 0.0
errorI = 0.
errorD = 0.
yaw = 0.0
goal = 20
base_yaw = 0.0
FIRST_ITERATION = True
imu_data_flag = False

print topicHeader.ABSOLUTE_RPY

def yaw_control(params):

    print "entered yaw-control"

    global errorI
    global errorP
    global errorD
    global imu_data_flag

    def abrpyCB(abrpy):
        global errorI
        global errorP
        global errorD
        global prevError
        global FIRST_ITERATION
        global base_yaw
        global imu_data_flag

        # print "Entered ABRPY callback"

        # yaw = float(raw_input("Enter yaw: "))

        yaw = abrpy.yaw

        if FIRST_ITERATION:

            base_yaw = yaw
            FIRST_ITERATION = False

        prevError = errorP
        # errorP should be four quadrant arc tangent so numpy.arctan2(sin(errorP),cos(errorP))
        errorP = base_yaw + goal - yaw
        errorP = numpy.arctan2(sin(errorP),cos(errorP))
        # print errorP
        errorI = errorP + prevError
        errorD = errorP - prevError
        # print errorP

        imu_data_flag = True

    rospy.init_node('twiddle_algm_node', anonymous=True)
    sub = rospy.Subscriber(topicHeader.ABSOLUTE_RPY, absoluteRPY, abrpyCB)
    pub4 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER4, thrusterData4Thruster, queue_size = 2)
    pub6 = rospy.Publisher(topicHeader.CONTROL_PID_THRUSTER6, thrusterData6Thruster, queue_size = 2)

    Kp = params[0]
    Ki = params[1]
    Kd = params[2]

    err = 0.

    i = 0

    while (i < 2*N):

        thruster6Data = thrusterData6Thruster()
        thruster4Data = thrusterData4Thruster()

        thruster6Data.data[0] = 0.0
        thruster6Data.data[1] = 0.0
        thruster6Data.data[2] = 0.0
        thruster6Data.data[3] = 0.0

        thruster6Data.data[4] = Kp*errorP + Kd*errorD + Ki*errorI
        thruster6Data.data[5] = -1 * thruster6Data.data[4]

        thruster4Data.data[0] = thruster6Data.data[0]
        thruster4Data.data[1] = thruster6Data.data[1]
        thruster4Data.data[2] = thruster6Data.data[4]
        thruster4Data.data[3] = thruster6Data.data[5]

        # pub4.publish(thruster4Data)

        pub6.publish(thruster6Data)

        # Run the twiddle loop 200 times
        # the first 100 times only control step will take place
        # and the next 100 times, the error will be noted down.
        if imu_data_flag:
                
            i += 1

            # print i
    
            if i >= N:
    
                    err += (errorP**2)

                    # print i, "th loop: Current accumulated error: ", err

            imu_data_flag = False

    be = err/N;

    print "Best error: ", round(be, 2)
    # temp = raw_input("Press enter to continue")

    return err / N;

def twiddle(tol = 0.001):
            
    n_params = 3
    dparams = [2.5 for row in range(n_params)]
    params  = [0.0 for row in range(n_params)]

    best_error = yaw_control(params)

    n = 0
    
    while sum(dparams) > tol:
        for i in range(len(params)):
            params[i] += dparams[i]
            print "params[", i, "]: ", params[i]
            err = yaw_control(params)
            if err < best_error:
                best_error = err
                dparams[i] *= 1.1
            else:
                params[i] -= 2.0 * dparams[i]
                err = yaw_control(params)
                if err < best_error:
                    best_error = err
                    dparams[i] *= 1.1
                else:
                    params[i] += dparams[i]
                    dparams[i] *= 0.9
        n += 1

        print 'Twiddle #', n, params, ' -> ', best_error, ' dparams: ', dparams

        temp = raw_input("Press enter to continue")

    print ''
    return params
    
params = twiddle()
# err = controller.yaw_control(params, True)
# print '\nFinal parameters: ',params, '\n ->' ,err
print "Final parameters: ", params
