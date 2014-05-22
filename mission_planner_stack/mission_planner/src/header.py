
############################CONSTANTS AND SERVICES NAMES##################################3
from enum import Enum
import rospy

ADVANCED_CONTROLLER_SERVER='advanced'
CONTROLLER_SERVER='CONTROLLER'
RESET_POSE_SERVICE='POSE_RESET'
ESTIMATED_POSE_TOPIC_NAME='/kraken/pose_estimated'
MARKER_DETECT_ACTION_SERVER='MARKER_SERVER'
BUOY_DETECT_ACTION_SERVER='BUOY'
VALIDATION_GATE_ACTION_SERVER='VALIDATE'
PREMAP_LOCATION_SERVICE='LOCATION'
IP_CONTROLLER_PUBLISHING_TOPIC='TOPIC'
MOVE_ALONG_SERVICE_NAME='MOVE-ALONG'
class PoseEnum():
    x=0
    y=1
    z=2
    vx=3
    vy=4
    vz=5
    ax=6
    ay=7
    az=8
    w_roll=9
    w_pitch=10
    w_yaw=11
    roll=12
    pitch=13
    yaw=14

def main():
    rospy.init_node("header_node")
    rospy.loginfo("heyy %d",PoseEnum.ax);

if __name__=='__main__':
    main()
    
    


##########################################################################################3
#'px'=0,'py'=1,'pz'=2,'vx'=3,'vy'=4,'vz'=5,'ax'=6,'ay'=7,'az'=8,'w_roll'=9,'w_pitch'=10,'w_yaw'=11,'r'=12,'p'=13,'y'=14
# 'px','py','pz','vx','vy','vz','ax','ay','az','w_roll','w_pitch','w_yaw','r','p','y'
# _px=0,
#     _py,
#     _pz,
#     
#     _vx,
#     _vy,
#     _vz,
#     
#     _ax,
#     _ay,
#     _az,
#     
#     _w_roll,
#     _w_pitch,
#     _w_yaw,
#     
#     _roll,
#     _pitch,
#     _yaw