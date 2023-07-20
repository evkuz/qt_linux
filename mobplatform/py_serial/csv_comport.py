#!/usr/bin/env python
# Пишем данные в файл в формате csv
# PC => mobPlat
# rsync -av /home/ubuntu/pyprojects/mobplatform/py_serial/bin_comport.py nvidia@192.168.1.176:/home/nvidia/pyprojects/mobplatform/py_serial/
#
# mobplat => PC
# rsync -av nvidia@192.168.1.176:/home/nvidia/pyprojects/mobplatform/py_serial/Et.txt /home/ubuntu/pyprojects/mobplatform/py_serial/plotting
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
#     int E                // 2 bytes
#     int Eprev            // 2 bytes


# }; 28 bytes total size
import serial
import threading
import serial.tools.list_ports
from threading import Event
import time
import struct
import math
import csv


def write_serial():
    global notFinished
    global event
    command = input()
    if command == "close":
        f.close()
        encdata.close()
        ser.flushInput()
        ser.flushOutput()
        ser.close()
        event.set()
        notFinished = False
        # t1.join()
        # t2.join()
        print("Bye...")
        exit()

    ser.write(command.encode())     # write a string
    # datastr = ser.readline()
    # print(datastr)


def read_serial():
    global diff
    global diff_prev
    global diff_posAm1
    global diff_popsAm2
    global posAm1_prev
    global posAm2_prev

    global Kp
    global Kd
    global Ki
    global Eintegral

    global csvData

    data = []
    mycsvData = []
    strData = ''
    while ser.in_waiting:
        # data = ser.read(10)
        # print(bytes.fromhex(data).decode('utf-8'))

        data = ser.read(40)
        encA1 = data[0:2]
        posA1, = struct.unpack('<h', encA1)
        mycsvData.append(posA1)

        encA2 = data[2:4]
        posA2, = struct.unpack('<h', encA2)
        mycsvData.append(posA2)

        Er = data[24:26]
        E, = struct.unpack('<h', Er)
        mycsvData.append(E)

        Epr = data[26:28]
        Eprev, = struct.unpack('<h', Epr)
        mycsvData.append(Eprev)

        long_millis = data[12:16]
        millis, = struct.unpack('<L', long_millis)
        mycsvData.append(millis)

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

        Integral = data[36:40]
        fIntegral, = struct.unpack('<f', Integral)
        P = Kp*E
        mycsvData.append(P)

        py_dedt = deltaE/f_deltaT
        # strData += "py_dedt= " + str(py_dedt) + ", "

        Eintegral = Eintegral + (E * f_deltaT)
        if abs(deltaE) < encodersGAP:
            I = 0.0
            Eintegral = 0.0
        else:
            I = Ki*fIntegral
        # strData += "I= " + str(I) + ", "
        mycsvData.append(I)

        D = Kd*py_dedt
        # strData += "D= " + str(D) + ", "
        mycsvData.append(D)

        csvData.writerow(mycsvData)
        mycsvData.clear()

        u = abs(P + I + D)
        if math.isnan(u):
            u = 0.0
        else:
            u = round(u)
        strData += "u= " + str(u) + "\n"

        # f.write(strData)
        # strData = str(posA1) + "," + str(posA2) + "," + str(millis) + str(diff) + '\n'
        # strData = str(millis) + "," + str(E) + '\n'
        # gr.write(strData)

        posAm1_prev = posA1
        posAm2_prev = posA2
        diff_prev = deltaE


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
    diff = 0
    diff_prev = 0   # Значение diff на предыдущем проходе потока
    diff_posAm1 = 0
    posAm1_prev = 0

    diff_popsAm2 = 0
    posAm2_prev = 0

    encodersGAP = 10
    Eintegral = 0
    Kp = 0.35
    Kd = 0.1
    Ki = 0.013

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
    gr = open("Et.txt", "wt")  # open("graph.txt", "wt")

    encdata = open('data.csv', 'w', encoding='UTF8', newline='')
    csvData = csv.writer(encdata)   #   ,delimiter= ',', quotechar= '|', quoting=csv.QUOTE_MINIMAL)
    header = ['posA1', 'posA2', 'E', 'Eprev', 'millis', 'm1Speed', 'm2Speed', 'dltE', 'deltaT', 'de/dt', 'P', 'I', 'D']
    mystr = [487, 478, 9, 0, 23544, 54, 54, 9, 0.2479, -40.4897, 5.9499, -1.2529, -3.2391]
    csvData.writerow(header)
    # csvData.writerow(mystr)
    # encdata.close()
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
            gr.close()
            encdata.close()
            break
        pass
    print("Bye...")
    exit()

