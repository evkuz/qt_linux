#!/usr/bin/env python
# Тут пишем данные в файл в отладочном виде
# Нужно соответствующая прошивка Ардуино
# PC => mobPlat
# rsync -av /home/ubuntu/pyprojects/mobplatform/py_serial/bin_comport.py nvidia@192.168.1.176:/home/nvidia/pyprojects/mobplatform/py_serial/
#
# mobplat => PC
# rsync -av nvidia@192.168.1.176:/home/nvidia/pyprojects/mobplatform/py_serial/graph.txt /home/ubuntu/pyprojects/mobplatform/py_serial/
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
#     unsigned long time_ms; // 32 bits current time in ms
#     float deltaT; // 32 bits
#     float dedt;   // 32 bits
#     int E;        // 2 bytes
#     int Eprev;    // 2 bytes
#     int lagSpeed_ptr; // 2 bytes
#     int fwdSpeed_ptr; // 2 bytes
#     int M1Speed_ptr;  // 2 bytes
#     int M2Speed_ptr;  // 2 bytes
#     float Integral;   // 4 bytes

# }; 40 bytes total size
import serial
import threading
import serial.tools.list_ports
from threading import Event
import time
import struct


def write_serial():
    global notFinished
    global event
    global counter
    global status

    if status == "stopped":
        status = "running"
        command = "moveon" #    input()
        ser.write(command.encode())
    # if command == "close":
        read_serial()
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

    #ser.write(command.encode())     # write a string

    # datastr = ser.readline()
    # print(datastr)


def read_serial():
    global diff
    global diff_prev
    global diff_posAm1
    global diff_popsAm2
    global posAm1_prev
    global posAm2_prev

    data = []
    while ser.in_waiting:
        # data = ser.read(10)
        # print(bytes.fromhex(data).decode('utf-8'))

        strData = str(ser.readline().strip())

        #   strData = "Абырвалг\n"
        # data = ser.read(24)
        # encA1 = data[0:2]
        # # posA1 = data[1]
        # # posA1 <<= 8
        # # posA1 += data[0]
        # posA1, = struct.unpack('<h', encA1)
        #
        # encA2 = data[2:4]
        # # posA2 = data[3]
        # # posA2 <<= 8
        # # posA2 += data[2]
        # posA2, = struct.unpack('<h', encA2)
        #
        # sp1 = data[8:10]
        # # m1Speed = data[9]
        # # m1Speed <<= 8
        # # m1Speed += data[8]
        # m1Speed, = struct.unpack('<h', sp1)
        #
        # m2Speed = data[11]
        # m2Speed <<= 8
        # m2Speed += data[10]
        #
        # long_millis = data[12:16]
        # millis, = struct.unpack('<l', long_millis)
        #
        # deltaT = data[16:20]
        # f_deltaT, = struct.unpack('<f', deltaT)
        #
        # dedt = data[20:24]
        # f_dedt, = struct.unpack('<f', dedt)
        #
        # diff = abs(posA1 - posA2)
        # diff_posAm1 = posA1 - posAm1_prev
        # diff_popsAm2 = posA2 - posAm2_prev

        # strData = "posA1= " + str(posA1) + ", "
        # strData += "posA2= " + str(posA2) + ", "
        # strData += "E= " + str(diff) + ", "
        # strData += "dt_diff= " + str(diff - diff_prev) + ", "
        # strData += "m1Speed= " + str(m1Speed) + ", "
        # strData += "m2Speed= " + str(m2Speed) + '\n'
        # strData += "diff_posAm1= " + str(diff_posAm1) + ", "
        # strData += "diff_posAm2= " + str(diff_popsAm2) + ", "
        #
        # strData += "millis = " + str(millis) + ", "
        #
        # strData += "delta_T = " + str(f_deltaT) + ", "
        # strData += "de/dt = " + str(f_dedt) + '\n'
        strData += '\n'
        f.write(strData)
        #   f.write("Super Puper !!!")
        # strData = "TEST"  # str(posA1) + "," + str(posA2) + "," + str(millis) + str(diff) + '\n'
        # gr.write(strData)


def perform_transactions():
    t2 = threading.Thread(target=read_serial)
    t1 = threading.Thread(target=write_serial)

    # start threads
    t2.start()
    t1.start()
    # wait until threads finish their job
    t2.join()
    t1.join()


if __name__ == "__main__":
    # print(serial.__version__)
    diff = 0
    diff_prev = 0   # Значение diff на предыдущем проходе потока
    diff_posAm1 = 0
    posAm1_prev = 0

    diff_popsAm2 = 0
    posAm2_prev = 0

    counter = 0 # счетчик запусков тележки

    status = "stopped" # running
    parcelSize = 40 # Количество байт в посылке Ардуино->ПК

    print([comport.device for comport in serial.tools.list_ports.comports()])
    ser = serial.Serial('/dev/ttyACM0', 115200, timeout=None)  # open serial port
    print(ser.name)  # check which port was really used
    myStr = ser.readline()
    print(myStr)
    ser.reset_input_buffer()
    ser.reset_output_buffer()
    ser.flushInput()
    ser.flushOutput()

    f = open("demofile.txt", "wt")
    # f.write("Test DATA")
    # gr = open("graph.txt", "wt")
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
            f.close()
            break
        pass
    print("Bye...")
    exit()

