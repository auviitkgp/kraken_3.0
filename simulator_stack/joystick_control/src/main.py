# import roslib;roslib.load_manifest('mission_planner')

from Tkinter import *
import rospy
from std_msgs.msg import String
# from kraken_msgs import *

# import kraken_msgs

# f = kraken_msgs.msg.forceData6Thruster()

# forceData6Thruster f
# print type(f)

pub = rospy.Publisher('keyboard', String, queue_size=10)
rospy.init_node('talker', anonymous=True)

def tellRos(content):

    content_str = str(content)

    pub.publish(content_str)

# Force Defination
# force[0] = forward thruster on left side, positive value takes vehicle forwards
# force[1] = forward thruster on right side, positive value takes vehicle forwards
# force[2] = sway thruster on front side, positive value takes vehicle rightwards
# force[3] = sway thruster on back side, positive value takes vehicle rightwards
# force[4] = depth thruster on back side, positive value takes vehicle downwards
# force[5] = depth thruster on front side, positive value takes vehicle downwards

# Keyboard Control

# w - forward
# a - left
# s - backward
# d - right
# t - top (towards the surface)
# g - ground (towards the pool bottom)

string_dict = {0 : "left", 1 : "right", 2 : "backward", 3 : "forward", 4 :
               "top", 5 : "bottom"}

def create_callbacks(arg):
    
    def callback(ev=None):
        tellRos(string_dict[arg])

    return callback

def callback(ev=None):
    print 'callback called!'

root = Tk()

frame = Frame(root, width=100, height=100)

left = Button(frame, text="left", command=create_callbacks(0))
left.grid(row=1,column=0)

back = Button(frame, text="backward", command=create_callbacks(2))
back.grid(row=1,column=1)

right = Button(frame, text="right", command=create_callbacks(1))
right.grid(row=1,column=2)

forward = Button(frame, text="forward", command=create_callbacks(3))
forward.grid(row=0,column=1)

top = Button(frame, text="top", command=create_callbacks(4))
top.grid(row=0,column=3)

bottom = Button(frame, text="bottom", command=create_callbacks(5))
bottom.grid(row=1,column=3)

root.bind("a", create_callbacks(0))
root.bind("d", create_callbacks(1))
root.bind("s", create_callbacks(2))
root.bind("w", create_callbacks(3))
root.bind("t", create_callbacks(4))
root.bind("g", create_callbacks(5))

frame.pack()

root.mainloop()
