#! /bin/bash

cur_dir=${PWD##*/} 

if [[ $cur_dir != "mobile_camera" ]]; then
    echo "Please run it from 'mobile_camera' directory. Abort..."
    exit 1
fi

cd ../../
rsync -av --exclude '__pycache__' --exclude '.venv' agents_python pi@192.168.1.201:~/

ssh pi@192.168.1.201 'sudo systemctl restart robot.camera.service'
echo $?