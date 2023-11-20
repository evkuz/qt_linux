#!/usr/bin/env python
# Запускаем последовательно разные команды.
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
#   В режиме "на полу" коэффициенты :
#   Kp = 0.0235
#   Ki = 0.0123
#   Kd = 0.0275

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


#  String commandList[9] = {"blink","start", "stop", "moveon", "setPID", "reset", "getValues" ,"samplingTime", "calibration"};

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
    global M1Speed
    global M2Speed
    global E_FinList
    global Etotal
    global M1Stat
    global M2Stat
    global Estat
    global E_AverageList

 #   print("I'm in parseData")
    mycsvData = []
    strData = ''

    encA1 = data[0:2]
    posA1, = struct.unpack('<h', encA1)
    mycsvData.append(posA1)
 #   print()  # Делаем пустую строку вывода

    encA2 = data[2:4]
    posA2, = struct.unpack('<h', encA2)
    mycsvData.append(posA2)

    diff = data[4:6]
    #   Er = data[24:26]
    E, = struct.unpack('<h', diff)
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
    # M1Speed.append(m1Speed)

    m2Speed = data[11]
    m2Speed <<= 8
    m2Speed += data[10]
    mycsvData.append(m2Speed)
    # M2Speed.append(m2Speed)

    deltaE = E - Eprev
    mycsvData.append(deltaE)

    deltaT = data[16:20]
    f_deltaT, = struct.unpack('<f', deltaT)
    mycsvData.append(f_deltaT)

    dedt = data[20:24]
    f_dedt, = struct.unpack('<f', dedt)
    mycsvData.append(f_dedt)

    sp1 = data[28:30]  # lagSpeed_ptr
    SPD1, = struct.unpack('<h', sp1)
    mycsvData.append(SPD1)

    sp2 = data[30:32]  # fwdSpeed_ptr
    SPD2, = struct.unpack('<h', sp2)
    mycsvData.append(SPD2)

    lagSpeed = data[32:34]  # "M1Speed_start" но мы здесь поставим адрес указателя lagmSpeed
    lgSpeed, = struct.unpack('<h', lagSpeed)
    mycsvData.append(lgSpeed)

    fwdSpeed = data[34:36]
    fdSpeed, = struct.unpack('<h', fwdSpeed)
    mycsvData.append(fdSpeed)

    P = Kp * E
    mycsvData.append(P)

    Integral = data[36:40]
    fIntegral, = struct.unpack('<f', Integral)
    #   Eintegral = Eintegral + E*f_deltaT
    #   fIntegral = Ki * Eintegral
    mycsvData.append(fIntegral)

    py_dedt = deltaE / f_deltaT
    D = Kd * py_dedt
    mycsvData.append(D)

    myStatus = data[40:48]
    lstatus, = struct.unpack('<8s', myStatus)
    #   print(lstatus)
    # После установки ПИД-коэффициентов в ответ приходит "Got-PId"
    if (lstatus.startswith(b'stopPO')) or (status == "stop") or (lstatus.startswith(b'Got')):
        Etotal.append(E)
        counter += 1

        if status == "moving":
            # print("!!!!!!!!!!!!!!!!!!!!! IAM IN STATISTICS !!!!!!!!!!!!!!!!!!!!!!!!!!!!!1")
            # from statistics import mean
            # print("M1Speed [min, max, average] :", min(M1Speed), max(M1Speed), round(mean(M1Speed)))
            # print("M2Speed [min, max, average] :", min(M2Speed), max(M2Speed), round(mean(M2Speed)))
            # print("M1Speed ", M1Speed)
            # print("M2Speed ", M2Speed)
            # print("Etotal ", Etotal)

            M1List = []
            M1List.append(min(M1Speed))
            M1List.append(max(M1Speed))
            M1List.append(round(mean(M1Speed)))
            M1Stat.append(M1List)
            M1Speed.clear()

            M2List = []
            M2List.append(min(M2Speed))
            M2List.append(max(M2Speed))
            M2List.append(round(mean(M2Speed)))
            M2Stat.append(M2List)
            M2Speed.clear()

            Elist = []
            Elist.append(min(Etotal))
            Elist.append(max(Etotal))
            Elist.append(round(mean(Etotal)))
            E_AverageList.append(round(mean(Etotal)))

            Estat.append(Elist)
            Etotal.clear()

            status = "stop"
            E_FinList.append(E)
            print(counter)
            print("Final E on finish is ", str(E))

        #   if status == "calculate":
        status = "stop"
    # Записываем значения в течение цикла
    if lstatus.startswith(b'calibra') or lstatus.startswith(b'started') or lstatus.startswith(b'pid'):
        M1Speed.append(m1Speed)
        M2Speed.append(m2Speed)
        Etotal.append(E)

    if lstatus.startswith(b'done'):
        status = "done"
        print("Now status is \"done\"")

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

    kIntegral = data[56:60]  # Ki
    kkIntegral, = struct.unpack('<f', kIntegral)
    kkIntegral = "%.4f" % kkIntegral
    mycsvData.append(kkIntegral)

    myDeriv = data[60:64]  # Kd
    fDeriv, = struct.unpack('<f', myDeriv)
    fDeriv = "%.4f" % fDeriv
    mycsvData.append(fDeriv)
    #   print("Float value Kd " + str(fDeriv))

    myText = data[64:107]  # mytext
    myString, = struct.unpack('<43s', myText)
    mycsvData.append(myString)

    # print(myString)
    # print("status value is", status)

    csvData.writerow(mycsvData)
    mycsvData.clear()

    strData = str(millis) + "," + str(E) + '\n'
    f.write(strData)


def write_serial():
    global notFinished
    global event
    global counter
    global status
    global m1_light
    global m2_light

    if status == "stop":
        status = "moving"
        task = "start"  # "moveon"   input()
        ser.write(task.encode())

    if status == "setPID":
        task = "setPIDabc"
        ser.write(task.encode())
        # ser.flushInput()
        # ser.flushOutput()
        status = "stop"

    if status == "calibrating":
        print()
        print("send calibrating")
        task = "calibration"
        ser.write(task.encode())
        status = "calculate"  # Главное, что не stop и не moving. Начинаем считать среднее значение скорости.
        print("Calibrating is  Sent")

    if status == "setSpeedInit":
        print("send setSpeedInit")
        #   Вот тут добавляем ранее вычисленные средние значения.
        task = "setSpeedInit "
        task += str(m1_light)
        task += " "
        task += str(m2_light)
        ser.write(task.encode())
        status = "waiting"
        print("Init command for speed : ", task)
        # По завершении команды setSpeedInit В ардуино меняется статус "waiting" --> "done".

    if status == "moveon":
        task = "moveon"
        ser.write(task.encode())
        status = "waiting"

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
    #    global E_FinList

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
    # Список используемых статусов. Чтобы в них не запутаться.
    lstatus = ["stop", "setPID", "moving", "calibrating", "calculate", "done", "waiting", "moveon"]
    diff = 0
    diff_prev = 0  # Значение diff на предыдущем проходе потока
    diff_posAm1 = 0
    posAm1_prev = 0

    diff_popsAm2 = 0
    posAm2_prev = 0

    encodersGAP = 10
    Eintegral = 0
    Kp = 0.0185
    Ki = 0.0080 # 0.0083  # 0.0123
    Kd = 0.0175  # 0.0275

    Num = 0  # Количество циклов запуска платформы, отсчет с 0
    Tfull = 6000  # Время работы в мс 1 цикла запуска платформы.
    Tpid = 300  # Время работы PID в мс. Это интервал, в течение которого действуют текущие ПИД-коэффициенты.

    # counter = -1 # счетчик запусков тележки, встаёт в 0 после отправки ПИД-коэффициентов
    counter = -1
    status = "stop"  # running
    parcelSize = 107  # Количество байт в посылке Ардуино->ПК

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
    myStr += str(Kp)  # f'{Kp:.4f}' *10000
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
    myStr += " "

    print(myStr)
    # myStr = "moveon"
    mynum = ser.write(myStr.encode())
    print(mynum)
    mydata = []
    time.sleep(1)
    # data = ser.read(parcelSize)
    mydata = ser.read(parcelSize)
    print("Got some data from serial !")
    #   print(str(mydata))

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
    f = open("Et.txt", "wt")    # Нужно для построения графика
    statData = open("statData.txt", "at")
    # gr = open("Et.txt", "wt")
    # f.write("Test DATA")
    # gr = open("graph.txt", "wt")
    encdata = open('data.csv', 'w', encoding='UTF8', newline='')
    csvData = csv.writer(encdata)  # ,delimiter= ',', quotechar= '|', quoting=csv.QUOTE_MINIMAL)
    header = ['posA1', 'posA2', 'E', 'Eprev', 'millis', 'movingTime', 'm1Speed', 'm2Speed', 'dltE', 'deltaT', 'de/dt', \
              'SPD1', 'SPD2', 'lagSpeed', 'fwdSpeed', 'P', 'I', 'D', 'u', 'Status', 'Kp', 'Ki', 'Kd', 'debugInfo']
    #   mystr = [487, 478, 9, 0, 23544, 54, 54, 9, 0.2479, -40.4897, 5.9499, -1.2529, -3.2391]
    csvData.writerow(header)

    E_FinList = []      # Массив значений E в конце каждого цикла Num
    Etotal = []         # Список значений E  в течение одного цикла
    E_AverageList = []  # Список значений в течение текущего цикла

    parseData(mydata)

    # myStr = "Please enter a command for serial"
    # print(myStr)
    ser.flushInput()
    ser.flushOutput()

    # myStr = "moveon"
    # ser.write(myStr.encode())
    # time.sleep(1)
    # data = ser.read(parcelSize)

    # print("Sent moveon command")
    #   print(input())
    status = "stop"
    event = Event()
    notFinished = True
    #   while notFinished:
    # while counter < Num:
    #     t1 = threading.Thread(target=write_serial)
    #     t2 = threading.Thread(target=read_serial)
    #     # start threads
    #     t1.start()
    #     #   time.sleep(1)
    #     t2.start()
    #     # wait until threads finish their job
    #     t1.join()
    #     t2.join()
    ########################################### калибровка
    M1Speed = []
    M2Speed = []

    status = "calibrating"
    counter = 0
    Num = 0
    while counter < Num:
        # Пока идет calibrating имеем статус "calculate"
        # print("counter = ", counter, " status = ", status)
        t1 = threading.Thread(target=write_serial)
        t2 = threading.Thread(target=read_serial)
        # start threads
        t1.start()
        #   time.sleep(1)
        t2.start()
        # wait until threads finish their job
        t1.join()
        t2.join()
        if status == "stop":
            status = "calibrating"

    # print(M1Speed)
    # print(M2Speed)
    from statistics import mean
    #
    # m1_avg = mean(M1Speed)
    # m2_avg = mean(M2Speed)
    # print("Average value of the M1, M2 Speed with precision up to 3 decimal value:")
    # print(round(m1_avg, 3), ", ", round(m2_avg, 3))
    # m1_light = round(m1_avg)
    # m2_light = round(m2_avg)
    m1_light = 26
    m2_light = 23

    print("Init values for speed are ", m1_light, ", ", m2_light)
    ###########################################  Отправка вычисленных начальных значений скоростей
    status = "setSpeedInit"
    while status != "done":
        perform_transactions()

    #   print("Initial values for wheels speed are settled")
    ##########################################  запускаем робота, пид включается по необходимости.
    M1Speed = []
    M2Speed = []
    Etotal = []
    M1Stat = []
    M2Stat = []
    Estat = []
    status = "stop"  # По этому статусу запускается "start"
    #   status = "moveon"
    counter = 0
    Num = 5
    while counter < Num:
        perform_transactions()
        # if status == "stop":
        #     status = "moveon"

    print(0)
    print("Banch of Start cycles is finished")
    E_FinList = E_FinList[-Num:]
    #   print(E_FinList)
    # abs_func = lambda x: abs(x)
    # resE = list(map(abs_func, E_FinList))
    # Берем абсолютную величину ошибки, чтобы среднее не портилось из-за знака.
    resE = list(map(abs, E_FinList))
    resultE = "E final values "
    resultE += str(resE)
    resultE += '\n'
    print("E final values ", resE)

    resE_avg = mean(resE)
    print("Average of the absolute values E :") #  with precision up to 3 decimal value
    print(round(resE_avg, 3))

    E_average_final = round(resE_avg)
    print("Average value of E on finish is ", E_average_final)
    print()  # Нужна пустая строка
    # Выводим значения PID-коэффициентов
    K_values = "Kp = "
    K_values += str(Kp)
    K_values += ", Ki = "
    K_values += str(Ki)
    K_values += ", Kd = "
    K_values += str(Kd)
    K_values += '\n'

    #   print("Kp = ", Kp, ", Ki = ", Ki, ", Kd = ", Kd)
    print(K_values)
    print("M1Speed statistics : ", M1Stat)
    print("M2Speed statistics : ", M2Stat)
    print("E value statistics : ", Estat)
    print("E average value list : ", E_AverageList)

    strE_Avg_fin = "Average value of E on finish is "
    strE_Avg_fin += str(E_average_final)
    strE_Avg_fin += '\n'

    strE_Avg = "E average value list : "
    strE_Avg += str(E_AverageList)
    strE_Avg += '\n'

    strEstat = "E value statistics : "
    strEstat += str(Estat)
    strEstat += '\n'

    strDelimeter ='==============================================\n'
    statData.write(strDelimeter)
    statData.write(K_values)
    statData.write(resultE)
    statData.write(strE_Avg_fin)
    statData.write(strEstat)
    statData.write(strE_Avg)

    f.close()
    statData.close()
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
    # pass
    print("Bye...")
    exit()
