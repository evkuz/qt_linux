#! /bin/bash

cur_dir=${PWD##*/} 

if [[ $cur_dir != "mobile_camera" ]]; then
    echo "Please run it from 'mobile_camera' directory. Abort..."
    exit 1
fi

cd ../../

USER=pi
ADDR=192.168.1.201

rsync -av --exclude '__pycache__' --exclude '.venv' agents_python/iqrdevice $USER@$ADDR:~/agents_python/
rsync -av --exclude '__pycache__' --exclude '.venv' agents_python/mobile_camera $USER@$ADDR:~/agents_python/
#rsync -av agents_python/requirements.txt $USER@$ADDR:~/agents_python/

ssh $USER@$ADDR 'sudo systemctl restart robot.camera.service'
echo $?