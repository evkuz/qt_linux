#! /bin/bash

cur_dir=${PWD##*/} 

if [[ $cur_dir != "blueman" ]]; then
    echo "Please run it from 'blueman' directory. Abort..."
    exit 1
fi

cd ../../

USER=ubuntu
ADDR=192.168.1.158

rsync -av --exclude '__pycache__' --exclude '.venv' agents_python/iqrdevice $USER@$ADDR:~/agents_python/
rsync -av --exclude '__pycache__' --exclude '.venv' agents_python/blueman $USER@$ADDR:~/agents_python/
#rsync -av agents_python/requirements.txt $USER@$ADDR:~/agents_python/

ssh $USER@$ADDR 'sudo systemctl restart robot.blueman.service'
echo $?