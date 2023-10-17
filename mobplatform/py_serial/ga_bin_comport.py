#!/usr/bin/env python
# Тут пишем данные в файл в текстовом виде, НО посылка приходит в двоичном.
# Нужно соответствующая прошивка Ардуино
# PC => mobPlat
# rsync -av /home/ubuntu/pyprojects/mobplatform/py_serial/ga_bin_comport.py nvidia@192.168.1.176:/home/nvidia/pyprojects/mobplatform/py_serial/
#
# mobplat => PC
# rsync -av nvidia@192.168.1.176:/home/nvidia/pyprojects/mobplatform/py_serial/Et.txt /home/ubuntu/pyprojects/mobplatform/py_serial/plotting
# rsync -av nvidia@192.168.1.176:/home/nvidia/pyprojects/mobplatform/py_serial/data.csv /home/ubuntu/pyprojects/mobplatform/py_serial
#
# Ищем файлы новее 01.07.2023
# sudo find /home/ubuntu -type f -newermt 2023-07-01 -iname "*.png" >> Bg2079.JUL
#
# Данные из компорта получаем в hex-виде 107 байт за посылку
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
#     char mystatus[8]; // 8 bytes
#     unsigned long timestamp; // 4 bytes
#     float Proportional; // 4 bytes
#     float Integral_k; // 4 bytes
#     float Derivative; // 4 bytes

#     }; // 107 bytes total

import serial
import threading
import serial.tools.list_ports
from threading import Event
import time
import struct
import math
import csv


def parseData(data):
    global diff
    global diff_prev
    global diff_posAm1
    global diff_popsAm2
    global posAm1_prev
    global posAm2_prev
    global Eintegral
    global parcelSize
    global csvData
    global status
    global counter

    print("I'm in parseData")
    mycsvData = []
    strData = ''

    encA1 = data[0:2]
    posA1, = struct.unpack('<h', encA1)
    mycsvData.append(posA1)
    print()

    encA2 = data[2:4]
    posA2, = struct.unpack('<h', encA2)
    mycsvData.append(posA2)

    diff = data[4:6]

    Er = data[24:26]
    E, = struct.unpack('<h', Er)
    mycsvData.append(E)

    Epr = data[26:28]
    Eprev, = struct.unpack('<h', Epr)
    mycsvData.append(Eprev)

    long_millis = data[12:16]
    millis, = struct.unpack('<L', long_millis)
    mycsvData.append(millis)

    movTime = data[48:52]
    movingTime, = struct.unpack('<l', movTime)
    mycsvData.append(movingTime)

    sp1 = data[8:10]
    m1Speed, = struct.unpack('<h', sp1)
    mycsvData.append(m1Speed)

    m2Speed = data[11]
    m2Speed <<= 8
    m2Speed += data[10]
    mycsvData.append(m2Speed)

    deltaE = E - Eprev
    mycsvData.append(deltaE)

    deltaT = data[16:20]
    f_deltaT, = struct.unpack('<f', deltaT)
    mycsvData.append(f_deltaT)

    dedt = data[20:24]
    f_dedt, = struct.unpack('<f', dedt)
    mycsvData.append(f_dedt)

    sp1 = data[28:30]
    SPD1, = struct.unpack('<h', sp1)
    mycsvData.append(SPD1)

    sp2 = data[30:32]
    SPD2, = struct.unpack('<h', sp2)
    mycsvData.append(SPD2)

    P = Kp * E
    mycsvData.append(P)

    Integral = data[36:40]
    fIntegral, = struct.unpack('<f', Integral)
    mycsvData.append(fIntegral)

    py_dedt = deltaE / f_deltaT
    D = Kd * py_dedt
    mycsvData.append(D)

    myStatus = data[40:48]
    lstatus, = struct.unpack('<8s', myStatus)
    print(lstatus)
    if (lstatus.startswith(b'stop')) or (status == "stop") or (lstatus.startswith(b'Got')): # После установки ПИД-коэффициентов в ответ приходит "Got-PId"
        counter += 1
        status = "stop"
        print(counter)

    I = fIntegral

    u = abs(P + I + D)
    if math.isnan(u):
        u = 0.0
    else:
        u = round(u)
    mycsvData.append(u)
    mycsvData.append(lstatus)

    myProp = data[52:56]  # Kp
    fProp, = struct.unpack('<f', myProp)
    fProp = "%.4f" % fProp
    mycsvData.append(fProp)

    myIntegral = data[56:60]
    fIntegral, = struct.unpack('<f', myIntegral)
    fIntegral = "%.4f" % fIntegral
    mycsvData.append(fIntegral)

    myDeriv = data[60:64]  # Kd
    fDeriv, = struct.unpack('<f', myDeriv)
    fDeriv = "%.4f" % fDeriv
    mycsvData.append(fDeriv)
    #   print("Float value Kd " + str(fDeriv))

    myText = data[64:107]  # Kd
    myString, = struct.unpack('<43s', myText)
    mycsvData.append(myString)

    csvData.writerow(mycsvData)
    mycsvData.clear()

    strData = str(millis) + "," + str(E) + '\n'
    f.write(strData)


def write_serial():
    global notFinished
    global event
    global counter
    global status

    if status == "stop":
        status = "moving"
        task = "start" # "moveon"   input()
        ser.write(task.encode())

    if status == "setPID":
        task = "setPIDabc"
        ser.write(task.encode())
        # ser.flushInput()
        # ser.flushOutput()
        status = "stop"

    if status == "close":
        read_serial(parcelSize)
        f.close()
        encdata.close()
        ser.flushInput()
        ser.flushOutput()
        ser.close()
        event.set()
        notFinished = False
        # t1.join()
        # t2.join()
        # print("Bye...")
        exit()


def read_serial():
    global diff
    global diff_prev
    global diff_posAm1
    global diff_popsAm2
    global posAm1_prev
    global posAm2_prev
    global Eintegral
    global parcelSize
    global csvData
    global status
    global counter

    data = []
    strData = ''

    while ser.in_waiting:
        data = ser.read(parcelSize)
        #   time.sleep(1)
        parseData(data)


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

    encodersGAP = 10
    Eintegral = 0
    Kp = 0.0235
    Ki = 0.0123
    Kd = 0.0275

    Num = 0     #   Количество циклов запуска платформы, отсчет с 0
    Tfull = 6000    # Время работы 1 цикла запуска платформы.
    Tpid = 200  #   Время работы PID в мс. Это интервал, в течение которого действуют текущие ПИД-коэффициенты.

   # counter = -1 # счетчик запусков тележки, встаёт в 0 после отправки ПИД-коэффициентов
    counter = -1
    status = "stop" # running
    parcelSize = 107 # Количество байт в посылке Ардуино->ПК

    print([comport.device for comport in serial.tools.list_ports.comports()])
    ser = serial.Serial('/dev/ttyACM0', 115200, timeout=None)  # open serial port
    print(ser.name)  # check which port was really used
    time.sleep(1)
    if ser.in_waiting:
        myStr = ser.readline()
        print(myStr)

    ser.reset_input_buffer()
    ser.reset_output_buffer()
    ser.flushInput()
    ser.flushOutput()

    myStr = "setPID "
    status = "setPID"
    myStr += str(Kp)     # f'{Kp:.4f}' *10000
    myStr += " "

    myStr += str(Ki)
    myStr += " "

    myStr += str(Kd)
    myStr += " "

    myStr += str(Num)
    myStr += " "

    myStr += str(Tfull)
    myStr += " "

    myStr += str(Tpid)

    print(myStr)
    # myStr = "moveon"
    mynum = ser.write(myStr.encode())
    print(mynum)
    mydata = []
    time.sleep(1)
    # data = ser.read(parcelSize)
    mydata = ser.read(parcelSize)
    print("Got some data from serial !")
    print(str(mydata))

    # while ser.in_waiting:
    #     data = ser.read(parcelSize)
    #     myStr = ser.read_all()
    #     print("!")
    #     ser.flushInput()
    #     ser.flushOutput()
    # ser.reset_input_buffer()
    # ser.reset_output_buffer()
    # print(str(data))

#    print("Go on !")
    f = open("Et.txt", "wt")
    # gr = open("Et.txt", "wt")
    # f.write("Test DATA")
    # gr = open("graph.txt", "wt")
    encdata = open('data.csv', 'w', encoding='UTF8', newline='')
    csvData = csv.writer(encdata)   #   ,delimiter= ',', quotechar= '|', quoting=csv.QUOTE_MINIMAL)
    header = ['posA1', 'posA2', 'E', 'Eprev', 'millis', 'movingTime', 'm1Speed', 'm2Speed', 'dltE', 'deltaT', 'de/dt',\
              'SPD1', 'SPD2', 'P', 'I', 'D', 'u', 'Status', 'Kp', 'Ki', 'Kd', 'debugInfo']
    #   mystr = [487, 478, 9, 0, 23544, 54, 54, 9, 0.2479, -40.4897, 5.9499, -1.2529, -3.2391]
    csvData.writerow(header)

    parseData(mydata)

    myStr = "Please enter a command for serial"
    print(myStr)
    ser.flushInput()
    ser.flushOutput()

    # myStr = "moveon"
    # ser.write(myStr.encode())
    # time.sleep(1)
    # data = ser.read(parcelSize)

    #print("Sent moveon command")
    #   print(input())
    status = "stop"
    event = Event()
    notFinished = True
    #   while notFinished:
    while counter < Num:
        t1 = threading.Thread(target=write_serial)
        t2 = threading.Thread(target=read_serial)
        # start threads
        t1.start()
        #   time.sleep(1)
        t2.start()
        # wait until threads finish their job
        t1.join()
        t2.join()


    #if event.is_set():
    f.close()
    encdata.close()
    ser.flushInput()
    ser.flushOutput()
    command = "stop"
    ser.write(command.encode())
    ser.flushInput()
    ser.flushOutput()
    ser.reset_output_buffer()
    ser.reset_input_buffer()
    ser.close()
    #    break
    #pass
    print("Bye...")
    exit()

