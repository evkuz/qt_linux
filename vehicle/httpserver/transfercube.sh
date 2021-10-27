#!/bin/bash

source /opt/ros/melodic/setup.bash
source ~/catkin_ws/devel/setup.bash

export ROS_IP=`hostname -I | awk '{print $1}'`
export ROS_IP=`ifconfig wlan0 |grep -v inet6 |grep inet | awk '{print $2}'`
export ROS_HOSTNAME=$ROS_IP
export ROS_MASTER_URI=http://$ROS_IP:11311

export XRBASE=tank
export XRLIDAR=rplidar

#Move to /home/xrrobot/catkin_ws/src/xrrobot_project/xrrobot/
cdx() {
        cd /home/xrrobot/catkin_ws/src/xrrobot_project/xrrobot/
}

python2.7 /home/xrrobot/sss/spoint.py