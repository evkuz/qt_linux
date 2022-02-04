#!/usr/bin/env python
# license removed for brevity

import sys
import rospy
import actionlib
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal

x1 = 0.750
y1 = 0.90
z1 = 0.0
w1 = 1.0

x2 = 0.10
y2 = 0.10
z2 = -0.7
w2 = 0.7


def movebase_client(x, y, z, w):
    result = -1

    client = actionlib.SimpleActionClient('move_base',MoveBaseAction)
    print('w8ing server')
    client.wait_for_server()
    print('ok')
    goal = MoveBaseGoal()
    goal.target_pose.header.frame_id = "map"
    goal.target_pose.header.stamp = rospy.Time.now()
    goal.target_pose.pose.position.x = x
    goal.target_pose.pose.position.y = y
    goal.target_pose.pose.position.z = 0.0
    goal.target_pose.pose.orientation.x = 0.0
    goal.target_pose.pose.orientation.y = 0.0
    goal.target_pose.pose.orientation.z = z
    goal.target_pose.pose.orientation.w = w

    client.send_goal(goal)
    wait = client.wait_for_result()
    if not wait:
        rospy.logerr("Action server not available!")
        rospy.signal_shutdown("Action server not available!")
    else:
        res = client.get_result()
        if res is not None:
            print "MoveBase result:", result
            result = 0
    return result
        

if __name__ == '__main__':
    result = 0
    print sys.argv
    try:
        rospy.init_node('movebase_client_py')
        if len(sys.argv) < 2:
            result = -2
        elif sys.argv[1] == 'a':
            result = movebase_client(x1,y1,z1,w1)
        elif sys.argv[1] == 'b':
            result = movebase_client(x2,y2,z2,w2)
        else:
            print("Unknown point name!")
            result = -1

        if result == 0:
            rospy.loginfo("Goal execution done!")
    except rospy.ROSInterruptException:
        rospy.loginfo("Navigation test finished.")

    exit(result)
