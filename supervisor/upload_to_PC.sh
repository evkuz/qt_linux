#! /bin/bash

cur_dir=${PWD##*/} 

DIRNAME="supervisor"

if [[ $cur_dir != $DIRNAME ]]; then
    echo "Please run it from '${DIRNAME}' directory. Abort..."
    exit 1
fi

cd ../

USER=ubuntu
ADDR=159.93.44.84
#ADDR=192.168.1.175

rsync -av --exclude '__pycache__' --exclude '.venv' $DIRNAME $USER@$ADDR:~/IQR/
#ssh $USER@$ADDR 'sudo systemctl restart ${DIRNAME}.service'
#echo $?