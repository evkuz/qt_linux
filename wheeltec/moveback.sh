#!/bin/bash

source /opt/ros/melodic/setup.bash
source /home/wheeltec/wheeltec_robot/devel/setup.bash

export ROS_MASTER_URI=http://192.168.1.80:11311
export ROS_HOSTNAME=192.168.1.80
export SVGA_VGPU10=0

python2.7 /home/wheeltec/sss/spoint_b.py