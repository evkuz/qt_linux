#!/usr/bin/env python
# license removed for brevity

import rospy
import actionlib
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal

a = 0.2 #расстояния в метрах
b = 0.5
c = 0.85
d = -1.0


def movebase_client(a, b, z, w):

    client = actionlib.SimpleActionClient('move_base',MoveBaseAction)
    print('w8ing server')
    client.wait_for_server()
    print('ok')
    goal = MoveBaseGoal()
    goal.target_pose.header.frame_id = "map"
    goal.target_pose.header.stamp = rospy.Time.now()
    goal.target_pose.pose.position.x = a
    goal.target_pose.pose.position.y = b
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
    try:
        rospy.init_node('movebase_client_py')
        for i in range(10):
            result = movebase_client(a, b, 0.7, 0.7)
            rospy.sleep(5)
            result = movebase_client(c, d, 1.0, 1.0)
            rospy.sleep(5)

        if result:
            rospy.loginfo("Goal execution done!")
    except rospy.ROSInterruptException:
        rospy.loginfo("Navigation test finished.")
