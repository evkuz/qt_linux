#! /bin/bash

cur_dir=${PWD##*/} 

if [[ $cur_dir != "mobman_move" ]]; then
    echo "Please run it from 'mobman_move' directory. Abort..."
    exit 1
fi

cd ../../

USER=xrrobot
ADDR=192.168.1.98

rsync -av --exclude '__pycache__' --exclude '.venv' agents_python/iqrdevice $USER@$ADDR:~/agents_python/
rsync -av --exclude '__pycache__' --exclude '.venv' agents_python/mobman_move $USER@$ADDR:~/agents_python/

ssh $USER@$ADDR 'sudo systemctl restart robot.mobman_move.service'
echo $?