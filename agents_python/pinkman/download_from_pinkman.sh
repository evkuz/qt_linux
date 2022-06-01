#! /bin/bash

cur_dir=${PWD##*/} 

if [[ $cur_dir != "pinkman" ]]; then
    echo "Please run it from 'pinkman' directory. Abort..."
    exit 1
fi

cd ../../

USER=ubuntu
ADDR=192.168.1.177

rsync -av --exclude '__pycache__' --exclude '.venv' $USER@$ADDR:~/agents_python/ agents_python/iqrdevice 
rsync -av --exclude '__pycache__' --exclude '.venv' $USER@$ADDR:~/agents_python/ agents_python/pinkman
rsync -av $USER@$ADDR:~/agents_python/requirements.txt agents_python/

sudo systemctl restart robot.pinkman.service
echo $?
