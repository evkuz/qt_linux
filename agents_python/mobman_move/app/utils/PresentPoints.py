#!/usr/bin/env python
# license removed for brevity

import sys
import rospy
import actionlib
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal

# Version 1. Robot with Manipulator
x1 = 0.0
y1 = 0.0
z1 = 1.0
w1 = 0.0

x2 = -1.10
y2 = -0.50
z2 = 0.70
w2 = -0.70

x3 = -1.10
y3 = -0.50
z3 = 0.20
w3 = 0.90

x0 = 0.0
y0 = 0.0
z0 = 0.0
w0 = 1.0


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
            #rospy.sleep(5)
            result = movebase_client(x2,y2,z2,w2)
            #rospy.sleep(5)
        elif sys.argv[1] == 'b':
            result = movebase_client(x3,y3,z3,w3)
            #rospy.sleep(5)
            result = movebase_client(x0,y0,z0,w0)
            #rospy.sleep(5)
        else:
            print("Unknown point name!")
            result = -1

        if result == 1:
            rospy.loginfo("Goal execution done!")
            result = 0
    except rospy.ROSInterruptException:
        rospy.loginfo("Navigation test finished.")

    exit(result)