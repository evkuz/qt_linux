#!/usr/bin/env python
# Попробуем оптимизировать работу компорта
# Все упирается в размер буфера ардуино - 32 байта. Если данных на отправку больше 32 байт, значит происходит
# более 1 посылки из ардуино в ПК, и, соответственно, буфер Tx в ардуино занят данными, а это буфер Rx в ПК.
# Значит условие "while ser.in_waiting:" почти все время срабатывает...
# Будем передовать из ардуино не строку, где явно более 32 байт
import serial
import serial.tools.list_ports
import time
print(serial.__version__)


def write_serial(command):
    ser.write(command.encode())     # write a string
    datastr = ser.readline()
    print(datastr)


print([comport.device for comport in serial.tools.list_ports.comports()])
ser = serial.Serial('/dev/ttyACM0',115200)  # open serial port
print(ser.name)         # check which port was really used
myStr = ser.readline()
print(myStr)
ser.reset_input_buffer()
ser.reset_output_buffer()

f = open("./demofile.txt", "a")
myStr = "Please enter a command for serial"
print(myStr)
i = 0
while 1:
    myInput = input()
    if myInput == "close":
        f.close()
        ser.flushInput()
        ser.flushOutput()
        ser.close()
        print("Bye...")
        exit()
    else:
        write_serial(myInput)
        while ser.in_waiting:
            myStr = ser.readline()
#           i+=1
#           line = str.decode('utf-8').replace('\r\n','')
            line = myStr.decode('utf-8')
#           time.sleep(0.1)
            f.write(line)    # Appends output to file +"\r\n"
            print(line)
            time.sleep(0.4)  # Time in seconds
#      f.write("qqq ")

#f.close()

#  print(myinput)
#  if myinput=="stop":
#    ser.write(b'stop')     # write a string
#    str = ser.readline()
#    print(str)
#    ser.close() 
#  if myinput=="start":
#    ser.write(b'start')     # write a string
#    str = ser.readline()
#    print(str)
#  Get encoders data
#  if myinput=="getValues":
#    ser.write(b'getValues')
#    str = ser.readline()
#    print(str)

