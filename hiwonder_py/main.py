# This is a sample Python script.
from PyQt5 import QtWidgets, uic
from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo
from PyQt5.QtCore import QIODevice  # , QByteArray
from PySide2.QtCore import QByteArray
import sys

import time

# /* Фиксированные позиции*/
hwr_Start_position = [93, 93, 93, 93, 93, 93]  # ; // servo1,,,servo6
sit_down_position = [93, 93, 93, 48, 48, 93]  # ; // Поза сидя. Сдвинуты 4,5 приводы (относительно 93)
horse_position = [93, 93, 0, 48, 48, 93]  # ;
horse_mirror_position = [45, 93, 180, 135, 135, 93]  # ;
ready_to_catch = [93, 93, 2, 15, 110, 93]  # ;
catch_box = [80, 93, 7, 10, 120, 93]  # ;
opposite_catch = [93, 93, 180, 125, 40, 60]
horse_1 = [93, 93, 170, 180, 163, 137]
horse_2 = [93, 93, 1, 11, 21, 137]

ready_to_catch_2 = [93, 93, 0, 27, 117, 51]
catch_box_2 = [80, 93, 11, 11, 117, 51]
ready_to_put_box_2 = [93, 93, 0, 4, 98, 175]
put_box_2 = [45, 93, 0, 7, 113, 180]
catch_box_3 = [80, 93, 0, 7, 113, 180]

# wrong_position = [93, 93, 2, 0, 10, 120]
wrong_position = [30, 30, 30, 30, 30, 30]

app = QtWidgets.QApplication([])
ui = uic.loadUi("form.ui")
ui.setWindowTitle("SerialGUI")

serial = QSerialPort()
serial.setBaudRate(115200)
portList = []
ports = QSerialPortInfo().availablePorts()
for port in ports:
    portList.append(port.portName())
# print(portList)
ui.comL.addItems(portList)


def onRead():
    if not serial.canReadLine():
        return  # выходим если нечего читать
    rx = serial.readLine()
    # rxs = str(rx, 'utf-8').strip()  # Данные в бинарном виде.
    # data = rxs.split(',')
    print(rx)


def onOpen():
    serial.setPortName(ui.comL.currentText())
    serial.open(QIODevice.ReadWrite)
    serial.clear()
    print("Serial buffer sise :")
    print(serial.readBufferSize())


def onClose():
    serial.close()


# Считываем данные из lineEdit в serialData
def prepare_data():
    for i in range(0, 6):
        serialData[i] = int(linedits[i].text())


def print_data_2_send():
    message = "Current data for Robot : "
    for i in range(0, 6):
        message += str(serialData[i])
        message += ", "
    message = message[:-2]
    print(message)


def onClamp():
    linedits[0].setText("45")
    serialData[0] = int(linedits[0].text())
    SerialSend(serialData)
    updateSliders()


def onStandUP():
    for i in range(0, 6):
        linedits[i].setText(str(hwr_Start_position[i]))
    # SerialSend(data)
    updateSliders()


def onGetBox():
    for i in range(0, 6):
        linedits[i].setText(str(ready_to_catch_2[i]))  # Встаем в позу перед захватом
    prepare_data()
    SerialSend(serialData)  # Go to down position
    # updateSliders()
    # Надо дождаться выполнения
    # wait for robot finish
    release_clamp()  # Открываем Захват
    # onClamp()
    # prepare_data()
    # updateSliders()
    # # Делаем захват
    for i in range(0, 6):
        linedits[i].setText(str(catch_box_2[i]))
    prepare_data()
    SerialSend(serialData)  # make catch_box
    # # Встаем в позу перед захватом
    for i in range(0, 6):
        linedits[i].setText(str(ready_to_catch_[i]))
    prepare_data()
    SerialSend(serialData)
    # Возврат на исходную
    for i in range(0, 6):
        linedits[i].setText(str(hwr_Start_position[i]))
    prepare_data()
    # print_data_2_send()
    SerialSend(serialData)


# =============================================================


def onPut_GetBox():
    for i in range(0, 6):
        linedits[i].setText(str(ready_to_catch_2[i]))  # Встаем в позу перед захватом
    prepare_data()
    SerialSend(serialData)  # Go to down position
    # updateSliders()
    # Надо дождаться выполнения
    # wait for robot finish
    release_clamp()  # Открываем Захват
    # onClamp()
    # prepare_data()
    # updateSliders()
    # # Делаем захват
    for i in range(0, 6):
        linedits[i].setText(str(catch_box_2[i]))
    prepare_data()
    SerialSend(serialData)  # make catch_box
    # # Встаем в позу перед захватом
    for i in range(0, 6):
        linedits[i].setText(str(sit_down_position[i]))
    prepare_data()
    SerialSend(serialData)
    # # Возврат на исходную
    for i in range(0, 6):
        linedits[i].setText(str(put_box_2[i]))
    prepare_data()
    # # print_data_2_send()
    SerialSend(serialData)


# Копируем фикс. данные позиции в текстровые окна
def onSitPosition():  # sit_down_position
    for i in range(0, 6):
        linedits[i].setText(str(sit_down_position[i]))
        prepare_data()
    # for i in range(0, 6):
    #     serialData[i] = int(linedits[i].text())
    # SerialSend(serialData)
    updateSliders()


def release_clamp():
    linedits[0].setText("45")
    prepare_data()
    SerialSend(serialData)


# Обработка нажатия кнопки "Set position"
def onSetPosition():
    for i in range(0, 6):
        serialData[i] = int(linedits[i].text())
        # print(serialData[i])
    # SerialSend(serialData)
    updateSliders()


def SerialSend(data):  # список инт
    txs = ""
    for val in data:
        txs += str(val)
        txs += ','
    txs = txs[:-1]
    txs += ';'
    # txs = ','.join(str(data)) + ';'
    print(txs)
    # serial.write(txs.encode())
    dd = bytes(data)
    qbdata = QByteArray(QByteArray.fromRawData(dd))
    # serial.clear()
    tx = serial.writeData(qbdata)


# Обработка сигнала textChanged
# def S1_data_changed():
#    txs = ui.servo_1_lineEdit.text()
#   print(txs)


# Обработка сигнала editingFinished
# Оставлю на память. Остальные не нужны
def S1_data_changed():
    txs = ui.servo_1_lineEdit.text()
    print(txs)


# Добавляем данные из LineEdit в sData для отправки роботу
def onSerial_getData(sData):
    # Собираем данные из servo_[1-6]_lineEdit, кладем их в массив
    # Это и будет точка взятия данных для нейронной сети

    for i in range(0, 6):
        sData[i] = linedits[i].text()

    return sData


# ============================= Slider group
def onS1_Slider():
    slval = ui.S1_verSlider.value()
    ui.servo_1_lineEdit.setText(str(slval))
    print(slval)
    #    serialData[0] = slval
    prepare_data()
    print_data_2_send()
    SerialSend(serialData)


def onS2_Slider():
    slval = ui.S2_verSlider.value()
    ui.servo_2_lineEdit.setText(str(slval))
    print(slval)
    #    serialData[1] = slval
    prepare_data()
    SerialSend(serialData)


def onS3_Slider():
    slval = ui.S3_verSlider.value()
    ui.servo_3_lineEdit.setText(str(slval))
    print(slval)
    #    serialData[2] = slval
    prepare_data()
    SerialSend(serialData)


def onS4_Slider():
    slval = ui.S4_verSlider.value()
    ui.servo_4_lineEdit.setText(str(slval))
    print(slval)
    prepare_data()
    #   serialData[3] = slval
    SerialSend(serialData)


def onS5_Slider():
    slval = ui.S5_verSlider.value()
    ui.servo_5_lineEdit.setText(str(slval))
    print(slval)
    #    serialData[4] = slval
    prepare_data()
    SerialSend(serialData)


def onS6_Slider():
    slval = ui.S6_verSlider.value()
    ui.servo_6_lineEdit.setText(str(slval))
    print(slval)
    #    serialData[5] = slval
    prepare_data()
    SerialSend(serialData)


def updateSliders():
    for i in range(0, 6):
        sliders[i].setValue(int(linedits[i].text()))


def onHorse_1_Button():
    for i in range(0, 6):
        linedits[i].setText(str(horse_1[i]))
    prepare_data()
    SerialSend(serialData)
    updateSliders()


def onHorse_2_Button():
    for i in range(0, 6):
        linedits[i].setText(str(horse_2[i]))
    prepare_data()
    SerialSend(serialData)
    updateSliders()


serial.readyRead.connect(onRead)
ui.openButton.clicked.connect(onOpen)
ui.closeButton.clicked.connect(onClose)
ui.clampButton.clicked.connect(onClamp)
ui.stand_upButton.clicked.connect(onStandUP)
ui.set_posButton.clicked.connect(onSetPosition)
ui.sitButton.clicked.connect(onSitPosition)
ui.getBoxButton.clicked.connect(onGetBox)
# ==================================
ui.horse_1_Button.clicked.connect(onHorse_1_Button)
ui.horse_2_Button.clicked.connect(onHorse_2_Button)
ui.get_putBoxButton.clicked.connect(onPut_GetBox)
# =============== Slider group
ui.S1_verSlider.valueChanged.connect(onS1_Slider)
ui.S2_verSlider.valueChanged.connect(onS2_Slider)
ui.S3_verSlider.valueChanged.connect(onS3_Slider)
ui.S4_verSlider.valueChanged.connect(onS4_Slider)
ui.S5_verSlider.valueChanged.connect(onS5_Slider)
ui.S6_verSlider.valueChanged.connect(onS6_Slider)

linedits = [ui.servo_1_lineEdit, ui.servo_2_lineEdit, ui.servo_3_lineEdit, ui.servo_4_lineEdit,
            ui.servo_5_lineEdit, ui.servo_6_lineEdit]

sliders = [ui.S1_verSlider, ui.S2_verSlider, ui.S3_verSlider, ui.S4_verSlider, ui.S5_verSlider, ui.S6_verSlider]

# Создаем массив данных для   ОТПРАВКИ в порт
serialData = [int(ui.servo_1_lineEdit.text()), int(ui.servo_2_lineEdit.text()), int(ui.servo_3_lineEdit.text()),
              int(ui.servo_4_lineEdit.text()), int(ui.servo_5_lineEdit.text()), int(ui.servo_6_lineEdit.text())]

# А этот сигнал эмитируется каждый раз, когда в этом поле что-то меняется. т,е. если вводим 3 цифрцы, то среагирует
# 3 раза.
# ui.servo_1_lineEdit.textChanged.connect(S1_data_changed)

# Так лучше, т.к. сигнал срабатывает, когда уже фокус покинул это поле. Вместо реагирования на изменение
# Каждой цифры, которая вводится в этом поле.

ui.servo_1_lineEdit.editingFinished.connect(S1_data_changed)
# ui.servo_2_lineEdit.editingFinished.connect(S2_data_changed)
# ui.servo_3_lineEdit.editingFinished.connect(S3_data_changed)
# ui.servo_4_lineEdit.editingFinished.connect(S4_data_changed)
# ui.servo_5_lineEdit.editingFinished.connect(S5_data_changed)
# ui.servo_6_lineEdit.editingFinished.connect(S6_data_changed)


ui.show()
app.exec()

# def print_hi(name):
# Use a breakpoint in the code line below to debug your script.
#    print(f'Hi, {name}')  # Press Ctrl+F8 to toggle the breakpoint.


# Press the green button in the gutter to run the script.
# if __name__ == '__main__':
#    print_hi('PyCharm')

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
