#!/bin/bash

# Копируем код для Ардуино в папку с python-кодом для ПК
ARDUINO_PROJECT="calibrating"
rsync -arv /home/ubuntu/Arduino/$ARDUINO_PROJECT /home/ubuntu/pyprojects/mobplatform/arduino/

# Копируем весь проект в папку для git
rsync -arv /home/ubuntu/pyprojects/mobplatform /home/ubuntu/iqr_lit/

# Удаляем лишнее, на git это не нужно хранить. 
rm -rf /home/ubuntu/iqr_lit/mobplatform/py_serial/venv
rm -rf /home/ubuntu/iqr_lit/mobplatform/py_serial/.idea


