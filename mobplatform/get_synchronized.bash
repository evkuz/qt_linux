#!/bin/bash

rsync -arv /home/ubuntu/Arduino/binary_timer1A_serial /home/ubuntu/pyprojects/mobplatform/arduino/

rsync -arv /home/ubuntu/pyprojects/mobplatform /home/ubuntu/iqr_lit/

rm -rf /home/ubuntu/iqr_lit/mobplatform/py_serial/venv
rm -rf /home/ubuntu/iqr_lit/mobplatform/py_serial/.idea


