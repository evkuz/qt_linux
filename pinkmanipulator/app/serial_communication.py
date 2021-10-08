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
        self.__thread = None
        self.__isWorking = False
        self.__isFinished = False

    def set_port(self, port_name):
        wasOpened = False
        if self.__thread:
            self.stop_communication();
            wasOpened = True

        self.__sp.port = port_name
        if wasOpened:
            self.start_communication()

    @property
    def isFinished(self):
        self.last_access = time.time()
        return self.__isFinished

    def start_communication(self):
        self.last_access = time.time()
        if self.__thread is not None:
            return
        self.__thread = threading.Thread(target=self.__thread_work)
        self.__thread.start()
        time.sleep(0.05)

    def stop_communication(self):
        if self.__thread is not None:
            self.__isWorking = False
            self.__thread.join()

    def __thread_work(self):
        self.__isWorking = True
        self.__sp.open()

        while self.__isWorking:
            self.__isFinished = self.__get_is_finished()
            time.sleep(0.05)
            #the last 5 seconds stop the thread
            if time.time() - self.last_access > 5:
                break

        self.__sp.close()
        self.__thread = None
        self.__isWorking = False


    def send_command(self, x:int, y:int, dist:int):
        self.start_communication()

        if x >= 640:
            x = 639

        if y >= 480:
            y = 479
        
        msg = f"{x:04d} {y:04d} {dist:04d}\n"
        self.__sp.write(msg.encode("utf-8"))

    def __get_is_finished(self):
        ans = self.__sp.read(3)
        return True if ans[0] == 49 else False



if __name__ == '__main__':
    ports = SerialCommunication.serial_ports()
    for p in ports:
        print(p)

    s = SerialCommunication(port=ports[0])
    s.start_communication()
    
    s.send_command(100, 480, 100)
    
    for i in range(30):
        print(s.isFinished)
        time.sleep(0.5)

    s.stop_communication()