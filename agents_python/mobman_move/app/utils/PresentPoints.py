#!/usr/bin/env python
# license removed for brevity

import sys
import rospy
import actionlib
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal

ax = 0.2
ay = 0.5

bx = 0.85
by = -1.0


def movebase_client(x, y, z, w):

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
        return client.get_result()

if __name__ == '__main__':
    result = 0
    print sys.argv
    try:
        rospy.init_node('movebase_client_py')
        if len(sys.argv) < 2:
            result = -2
        elif sys.argv[1] == 'a':
            result = movebase_client(ax, ay, 0.7, 0.7)
        elif sys.argv[1] == 'b':
            result = movebase_client(bx, by, 1.0, 1.0)
        else:
            print("Unknown point name!")
            result = -1

        if result:
            rospy.loginfo("Goal execution done!")
    except rospy.ROSInterruptException:
        rospy.loginfo("Navigation test finished.")

    if result == 1:
        result = 0
        
    exit(result)
