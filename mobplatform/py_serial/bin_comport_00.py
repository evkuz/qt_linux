#!/usr/bin/env python
# Данные из компорта получаем в hex-виде 10 байт за посылку
# struct
# Enc {
#     word A1_Enc; 2 байта
#     word A2_Enc; 2 байта
#     short diff;  2
#     short diff_vector; 2
#     int  M1_Speed; 2 // Не более 255, знаем.Штатное значение - 100, поэтому для "торможения" или "обгона" 255
#                      //  должно хватить. да, но очень медленно. Делаем int [-32768; 32767]
#     int M2_Speed;  2
# };
import serial
import threading
import serial.tools.list_ports
from threading import Event
import time


def write_serial():
    global notFinished
    global event
    command = input()
    if command == "close":
        f.close()
        ser.flushInput()
        ser.flushOutput()
        ser.close()
        event.set()
        notFinished = False
        # t1.join()
        # t2.join()
        # print("Bye...")
        exit()

    ser.write(command.encode())     # write a string
    # datastr = ser.readline()
    # print(datastr)


def read_serial():
    while ser.in_waiting:
        # data = ser.read(10)
        # print(bytes.fromhex(data).decode('utf-8'))
        data = ser.readline()
        print(data)
        f.write(data)


def perform_transactions():

    t1 = threading.Thread(target=write_serial)
    t2 = threading.Thread(target=read_serial)

    # start threads
    t1.start()
    t2.start()
    # wait until threads finish their job
    t1.join()
    t2.join()


if __name__ == "__main__":
    # print(serial.__version__)
    print([comport.device for comport in serial.tools.list_ports.comports()])
    ser = serial.Serial('/dev/ttyACM0', 115200, timeout=None)  # open serial port
    print(ser.name)  # check which port was really used
    myStr = ser.readline()
    print(myStr)
    ser.reset_input_buffer()
    ser.reset_output_buffer()
    f = open("demofile.txt", "wb")
    myStr = "Please enter a command for serial"
    print(myStr)
    event = Event()
    notFinished = True
    while notFinished:
        #perform_transactions()
        t1 = threading.Thread(target=write_serial)
        t2 = threading.Thread(target=read_serial)
        # start threads
        t1.start()
        t2.start()
        # wait until threads finish their job
        t1.join()
        t2.join()
        if event.is_set():
            break
        pass
    print("Bye...")
    exit()

