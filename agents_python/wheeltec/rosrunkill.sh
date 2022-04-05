#!/bin/bash

source /opt/ros/melodic/setup.bash
source /home/wheeltec/wheeltec_robot/devel/setup.bash

export ROS_IP=`hostname -I | awk '{print $1}'`
export ROS_IP=`ifconfig wlan0 |grep -v inet6 |grep inet | awk '{print $2}'`
export ROS_HOSTNAME=$ROS_IP
export ROS_MASTER_URI=http://$ROS_IP:11311
export SVGA_VGPU10=0


export XRBASE=tank
export XRLIDAR=rplidar

rosnode kill -a