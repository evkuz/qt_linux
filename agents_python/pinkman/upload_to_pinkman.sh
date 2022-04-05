#! /bin/bash

cur_dir=${PWD##*/} 

if [[ $cur_dir != "pinkman" ]]; then
    echo "Please run it from 'pinkman' directory. Abort..."
    exit 1
fi

cd ../../

USER=ubuntu
ADDR=192.168.1.177

rsync -av --exclude '__pycache__' --exclude '.venv' agents_python/iqrdevice $USER@$ADDR:~/agents_python/
rsync -av --exclude '__pycache__' --exclude '.venv' agents_python/pinkman $USER@$ADDR:~/agents_python/
rsync -av agents_python/requirements.txt $USER@$ADDR:~/agents_python/

ssh $USER@$ADDR 'sudo systemctl restart robot.pinkman.service'
echo $?