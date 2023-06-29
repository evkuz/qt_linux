#!/usr/bin/env python
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
#     int E                // 2 bytes
#     int Eprev            // 2 bytes


# }; 28 bytes total size
import serial
import threading
import serial.tools.list_ports
from threading import Event
import time
import struct


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

    data = []
    while ser.in_waiting:
        # data = ser.read(10)
        # print(bytes.fromhex(data).decode('utf-8'))

        data = ser.read(40)
        encA1 = data[0:2]
        # posA1 = data[1]
        # posA1 <<= 8
        # posA1 += data[0]
        posA1, = struct.unpack('<h', encA1)

        encA2 = data[2:4]
        # posA2 = data[3]
        # posA2 <<= 8
        # posA2 += data[2]
        posA2, = struct.unpack('<h', encA2)

        serDiff = data[4:6]
        sDiff, =  struct.unpack('<h', serDiff)

        diff_vector = data[6:8]
        sDiff_vector, = struct.unpack('<h', diff_vector)

        sp1 = data[8:10]
        # m1Speed = data[9]
        # m1Speed <<= 8
        # m1Speed += data[8]
        m1Speed, = struct.unpack('<h', sp1)

        m2Speed = data[11]
        m2Speed <<= 8
        m2Speed += data[10]

        long_millis = data[12:16]
        millis, = struct.unpack('<l', long_millis)

        deltaT = data[16:20]
        f_deltaT, = struct.unpack('<f', deltaT)

        dedt = data[20:24]
        f_dedt, = struct.unpack('<f', dedt)

        Er = data[24:26]
        E, = struct.unpack('<h', Er)

        Epr = data[26:28]
        Eprev, = struct.unpack('<h', Epr)

        diff = abs(posA1 - posA2)
        diff_posAm1 = posA1 - posAm1_prev
        diff_popsAm2 = posA2 - posAm2_prev

        #encA = data[1] << 8 + data[0]
#        print("Got some data")
        #print(int(encA.hex(), base=16))
        # print("posA1= ", int(posA1))
        # print("posA2= ", int(posA2))
        # print("m1Speed= ", int(m1Speed))
        # print("m2Speed= ", int(m2Speed))
        #print(int(struct.unpack('>h', data[0:2])))
        strData = "posA1= " + str(posA1) + ", "
        strData += "posA2= " + str(posA2) + ", "

        strData += "E= " + str(E) + ", "
        strData += "Eprev= " + str(Eprev) + ", "

        strData += "m1Speed= " + str(m1Speed) + ", "
        strData += "m2Speed= " + str(m2Speed) + ", "

        ptrLag = data[28:30]
        ptrLg, = struct.unpack('<h', ptrLag)

        ptrFwd = data[30:32]
        ptrFd, = struct.unpack('<h', ptrFwd)

        ptrM1 = data[32:34]
        ptrA1, = struct.unpack('<h', ptrM1)

        ptrM2 = data[34:36]
        ptrA2, = struct.unpack('<h', ptrM2)

        lgdata = data[36:38]
        lagData, = struct.unpack('<h', lgdata)

        fwdata = data[38:40]
        fwdData, = struct.unpack('<h', fwdata)

        # strData += "ptr_m1Speed = " + hex(ptrA1) + ", "
        # strData += "ptr_m2Speed = " + hex(ptrA2) + ", "
        #
        # # strData += "ptr_LAGSpeed = " + hex(ptrLg) + ", "
        # strData += "ptr_FWDSpeed = " + hex(ptrFd) + ", "
        #
        # strData += "*ptr_LAGSpeed = " + str(lagData) + ", "
        # strData += "*ptr_FWDSpeed = " + str(fwdData) + "\n"

        deltaE = E - Eprev
        strData += "dltE= " + str(deltaE) + "\n "

        strData += "diff_posAm1= " + str(diff_posAm1) + ", "
        strData += "diff_posAm2= " + str(diff_popsAm2) + ", "

        strData += "millis = " + str(millis) + ", "

        strData += "delta_T = " + str(f_deltaT) + ", "
        strData += "de/dt = " + str(f_dedt) + ", "

        deltaE = E - Eprev
        P = Kp*E
        strData += "P = " + str(P) + ", "

        py_dedt = deltaE/f_deltaT
        strData += "py_dedt = " + str(py_dedt) + ", "

        Eintegral = Eintegral + (E * f_deltaT)
        if (diff <40):
            I = 0.0
            Eintegral = 0.0
        else:
            I = Ki*Eintegral
        strData += "I = " + str(I) + ", "

        D = Kd*py_dedt
        strData += "D = " + str(D) + ", "

        u = abs(P + I + D)
        #u = u/2
        u = round(u)
        strData += "u = " + str(u) + "\n"

        f.write(strData)
        # strData = str(posA1) + "," + str(posA2) + "," + str(millis) + str(diff) + '\n'
        strData = str(millis) + "," + str(E) + '\n'
        gr.write(strData)

        posAm1_prev = posA1
        posAm2_prev = posA2
        diff_prev = diff


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

    Eintegral = 0
    Kp = 0.15
    Kd = 0.05
    Ki = 0.001

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
    gr = open("e(t).txt", "wt")  # open("graph.txt", "wt")

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

