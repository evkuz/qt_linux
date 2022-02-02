#! /bin/bash

cur_dir=${PWD##*/} 

if [[ $cur_dir != "mobman_move" ]]; then
    echo "Please run it from 'mobman_move' directory. Abort..."
    exit 1
fi

cd ../../
rsync -av --exclude '__pycache__' --exclude '.venv' agents_python xrrobot@192.168.1.98:~/

ssh xrrobot@192.168.1.98 'sudo systemctl restart robot.mobman_move.service'
echo $?