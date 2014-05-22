import roslib;roslib.load_manifest('mission_planner')
import rospy
import smach
import smach_ros
import roslib 
import actionlib
from rospy.timer import sleep
from smach_ros.simple_action_state import SimpleActionState
from actionLibExample.msg._sequenceAction import sequenceAction
from actionLibExample.msg._sequenceGoal import sequenceGoal
# from actionLibExample.msg._sequenceGoal import sequenceFeedback
from actionlib.simple_action_client import SimpleActionClient
from actionlib_msgs.msg._GoalStatus import GoalStatus
     


def main():
    rospy.init_node('new_node')
    
    
    cm=smach.Concurrence(outcomes=['server_running','succeeded','time_out'], default_outcome='server_running',
                         outcome_map={'succeeded':{'SERVER_STATE':'succeeded'},
                                      'time_out':{'SERVER_STATE':'preempted'},
                                      'time_out':{'SERVER_STATE':'aborted'}
                                      })
    with cm:
        goal=sequenceGoal()
        goal.n=100
        smach.Concurrence.add('SERVER_STATE', SimpleActionState("sequence", sequenceAction,goal))
          
          
       
#     sis = smach_ros.IntrospectionServer('server_name', cm, '/SM_ROOT')
#     sis.start()
 
 
    outcome=cm.execute()
    if(outcome=='succeeded'):
        rospy.loginfo("hehe ")
    elif outcome=='server_running':
        rospy.logfatal("shit")
    
    else:
        rospy.logfatal('omg')
    rospy.loginfo("done working")


#     client=SimpleActionClient("sequence", sequenceAction)
#     goal=sequenceGoal()
#     goal.n=input("enter n")
#     client.send_goal(goal)
#     client.wait_for_result()
#     if client.get_state()==GoalStatus.SUCCEEDED:
#         rospy.loginfo("done")
    

    
    
if __name__ == "__main__":
    main()
    
    
            
        
        