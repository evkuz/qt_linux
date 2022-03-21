#!/usr/bin/env python

import sys
import rospy
import actionlib
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
from geometry_msgs.msg import Twist

# Presentation version 1 according to A.G
x1 = 0.750
y1 = 0.90
z1 = 0.0
w1 = 1.0

x2 = 0.00
y2 = 0.10
z2 = 0.0
w2 = 1.0

# Presentation version 2 according to A.R
x1 = 0.750
y1 = 0.90
z1 = 0.0
w1 = 1.0

x2 = 0.10
y2 = 0.10
z2 = -0.7
w2 = 0.7

# Presentation version 3 according to A.R + turn on 10,5 o'clock
x1 = 0.750
y1 = 0.90
z1 = 0.20
w1 = 0.90

x2 = 0.10
y2 = 0.10
z2 = -0.7
w2 = 0.7

# Presentation version 4 like v.3 but according technical process (reverse A-B)
#Point A
x1 = 0.10
y1 = 0.10
z1 = -0.7
w1 = 0.7

#Point B
x2 = 0.750
y2 = 0.90
z2 = 0.20
w2 = 0.90

# Presentation version 4.2 turn on 10 o'clock
#Point A
x1 = 0.10
y1 = 0.10
z1 = -0.7
w1 = 0.7

#Point B
x2 = 0.75
y2 = 0.9
z2 = 0.3
w2 = 0.8

# Presentation version 4.3 where starting point = point A
#Point A
x1 = 0.0
y1 = 0.0
z1 = 0.0
w1 = 1.0

#Point B
x2 = -0.75
y2 = 0.9
z2 = -0.3
w2 = 0.8


# Presentation version 5 according Mikhail (starting point match with point A)
#Point A
x1 = 0.23
y1 = 0.0
z1 = 0.0
w1 = 1.0

#Point B
x2 = 0.0
y2 = 0.0
z2 = 1.0
w2 = 0.0

x3 = -0.65
y3 = 0.70
z3 = 0.98
w3 = 0.15

# Presentation version 6 (more robust, starting point match with point A)
# Point B
x1 = 0.0
y1 = 0.0
z1 = 1.0
w1 = 0.0

x2 = -0.65
y2 = 0.0
z2 = 0.7
w2 = 0.7

x3 = -0.65
y3 = 0.70
z3 = 0.98
w3 = 0.15



#Point A
x4 = -0.65
y4 = 0.70
z4 = 0.7
w4 = -0.7

x5 = -0.65
y5 = 0.0
z5 = 0.0
w5 = 1.0

x6 = 0.23
y6 = 0.0
z6 = 0.0
w6 = 1.0



# Presentation version 6.2 (more precise intermediate point)
# Point B
x1 = 0.0
y1 = 0.0
z1 = 1.0
w1 = 0.0

x2 = -0.75
y2 = 0.0
z2 = 0.7
w2 = 0.7

x3 = -0.65
y3 = 0.70
z3 = 0.98
w3 = 0.15



#Point A
x4 = -0.65
y4 = 0.70
z4 = 0.7
w4 = -0.7

x5 = -0.75
y5 = -0.12
z5 = 0.0
w5 = 1.0

x6 = 0.23
y6 = -0.05
z6 = 0.0
w6 = 1.0



# Presentation version 7 (more robust, a path w/o pits)
# Point A
#x1 = 0.0
#y1 = 0.0
#z1 = 0.7
#w1 = 0.7

#x2 = 0.0
#y2 = 0.75
#z2 = 0.0
#w2 = 1.0

#x3 = 0.65
#y3 = 0.0
#z3 = 0.2
#w3 = 0.9

#Point B
#x4 = 0.0
#y4 = 0.0
#z4 = 0.0
#w4 = 1.0

##Move back

#x5 = 0.0
#y5 = 0.75
#z5 = -0.7
#w5 = 0.7

#x6 = 0.0
#y6 = 0.0
#z6 = -0.7
#w6 = 0.7

#Move back





'''
a = 1.0
b = 0.0
c = 0.0
d = 0.0
'''


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
            result = movebase_client(x4, y4, z4, w4)
 #           rospy.sleep(1)
            result = movebase_client(x5, y5, z5, w5)
#            rospy.sleep(1)
            result = movebase_client(x6, y6, z6, w6)
#            result = movebase_client(x4, y4, z4, w4)

        elif sys.argv[1] == 'b':            
            result = movebase_client(x1, y1, z1, w1)

            result = movebase_client(x2, y2, z2, w2)
#	    rospy.sleep(1)
            result = movebase_client(x3, y3, z3, w3)

        else:
            print("Unknown point name!")
            result = -1

        if result == 1:
            rospy.loginfo("Goal execution done!")
            result = 0
    except rospy.ROSInterruptException:
        rospy.loginfo("Navigation test finished.")

    exit(result)
