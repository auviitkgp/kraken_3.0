import rospy
from std_msgs.msg import String

pub = rospy.Publisher('keyboard', String, queue_size=10)
rospy.init_node('talker', anonymous=True)

def tellRos(content):

    content_str = str(content)

    pub.publish(content_str)