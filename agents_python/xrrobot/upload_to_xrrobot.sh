#! /bin/bash

cur_dir=${PWD##*/} 

if [[ $cur_dir != "xrrobot" ]]; then
    echo "Please run it from 'xrrobot' directory. Abort..."
    exit 1
fi

cd ../../

USER=xrrobot
ADDR=192.168.1.98

rsync -av --exclude '__pycache__' --exclude '.venv' agents_python/iqrdevice $USER@$ADDR:~/agents_python/
rsync -av --exclude '__pycache__' --exclude '.venv' agents_python/xrrobot $USER@$ADDR:~/agents_python/
rsync -av agents_python/requirements.txt $USER@$ADDR:~/agents_python/

ssh $USER@$ADDR 'sudo systemctl restart robot.xrrobot.service'
echo $?