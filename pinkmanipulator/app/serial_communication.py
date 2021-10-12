from sys import platform
from glob import glob
import serial
import threading
import time


class SerialCommunication:
    @staticmethod
    def serial_ports():
        """Lists serial port names

            :raises EnvironmentError:
                On unsupported or unknown platforms
            :returns:
                A list of the serial ports available on the system
        """
        if platform.startswith('win'):
            ports = ['COM%s' % (i + 1) for i in range(256)]
        elif platform.startswith('linux') or platform.startswith('cygwin'):
            # this excludes your current terminal "/dev/tty"
            ports = glob('/dev/tty[A-Za-z]*')
        elif platform.startswith('darwin'):
            ports = glob('/dev/tty.*')
        else:
            raise EnvironmentError('Unsupported platform')

        result = []
        for port in ports:
            try:
                s = serial.Serial(port)
                s.close()
                result.append(port)
            except (OSError, serial.SerialException):
                pass
        return result

    def __init__(self, port='/dev/usb/ttyUSB0', baudrate=115200):
        self.last_access = 0
        self.__sp = serial.Serial()
        self.__sp.port = port
        self.__sp.baudrate = baudrate
        self.__isOpened = False
        self.__currentState = (0, 0, 0, 0, 0)

    def set_port(self, port_name):
        wasOpened = False
        if self.__isOpened:
            self.close_device()
            wasOpened = True

        self.__sp.port = port_name
        if wasOpened:
            self.open_device()

    def get_state(self):
        self.__write_message("s\n")
        self.__currentState = self.__read_state()
        return self.__currentState

    def open_device(self):
        if not self.__isOpened:
            self.__sp.open()
            time.sleep(1)
            self.__isOpened = True
            self.go_to_start()

    def close_device(self):
        if self.__isOpened:
            self.__sp.close()
            self.__isOpened = False

    def send_command(self, pos1:int, pos2:int, pos3:int, pos4:int):
        msg = f"m {pos1:03d} {pos2:03d} {pos3:03d} {pos4:03d}\n"
        self.__write_message(msg)
        self.__currentState = self.__read_state()
        return self.__currentState

    def go_to_start(self):
        return self.send_command(91, 140, 35, 170)

    def __write_message(self, msg):
        if not self.__isOpened:
            raise RuntimeError("Device not opened!")
        self.__sp.write(msg.encode("utf-8"))

    def __read_state(self):
        if not self.__isOpened:
            raise RuntimeError("Device not opened!")

        line = self.__sp.readline()#str(, "utf-8")
        res = line.decode("utf-8").split('\t')
        if len(res) == 5:
            return (int(res[0]), int(res[1]), int(res[2]), int(res[3]), int(res[4]))
        else:
            return (0, 0, 0, 0, 0)



if __name__ == '__main__':
    ports = SerialCommunication.serial_ports()
    for p in ports:
        print(p)

    s = SerialCommunication(port=ports[0])
    s.open_device()
    time.sleep(2)
    while True:
        print(s.get_state())
        line = input()
        
        if len(line) < 2:
            break
        res = line.split(',')
        if len(res) == 4:
            s.send_command(int(res[0]), int(res[1]), int(res[2]), int(res[3]))

    s.close_device()