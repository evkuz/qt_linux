from sys import platform
from glob import glob
import serial
import threading
import time
import logging
from .basecontroller import BaseController
from typing import List


default_timeout = 2.


class ArduinoManipulator(BaseController):
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

    def __init__(self, name:str="Serial"):
        BaseController.__init__(self, name)

        self.__sp = serial.Serial(timeout=default_timeout*0.5)
        self.__n_servos = 0
        self.__currentState =  ([], 0.)
        
        self.__readingThread = None
        self.__isFreeEvent = threading.Event()
        self.__isFreeEvent.set()
        self.__commandAcceptedEvent = threading.Event()
        self.__stateRecivedEvent = threading.Event()
        self.__endMovingEvent = threading.Event()
        self.logger = logging.getLogger(__name__)
    
    @property
    def channels(self) -> List[str]:
        return ['position', 'moving']

    def __del__(self):
        self.close_device()

    def __enter__(self):
        return self
    
    def __exit__(self, exc_type, exc_val, exc_tb):
        self.close_device()

    def open_device(self, port, baudrate:int=115200):
        """Opens serial port with given port and baudrate

        Args:
            port (int or str): If type is int it is port number in
            list of serial ports, if type is string it is exact name of port 
            
            baudrate (int, optional): Baudrate of serial port. Defaults to 115200.
        """
        if self.is_open:
            self.close_device()
        
        if type(port) is int:
            portList = ArduinoManipulator.serial_ports()
            port = portList[port]
        
        self.logger.info(f"Opening {self._name} port={port} ({baudrate})...")
        self.__sp.port = port
        self.__sp.baudrate = baudrate
        self.__sp.open()
        if self.is_open:
            self.__readingThread = threading.Thread(target=self.__reading_port_thread_work)
            self.__readingThread.start();
        self.logger.info(f"{self._name} port is open: {self.is_open}")
        time.sleep(1.)

    def close_device(self):
        if self.is_open:
            if not self.__endMovingEvent.is_set():
                self.stop_moving()
    
        self.__sp.close()
        self.__endMovingEvent.set()
        self.__isFreeEvent.set()
        self.logger.info(f"{self._name} port was closed")
    
    @property
    def is_open(self):
        return self.__sp.is_open

    @property
    def n_servos(self):
        return self.__n_servos

    def move(self, positions:list, speed:int=50, wait:bool=False):
        if len(positions) != self.__n_servos:
            raise RuntimeError("Wrong number of positions!")

        msg = 'm ' + " ".join(map(str, positions)) + ' ' + str(speed) + '\n'
        self.__endMovingEvent.clear()
        self.__send_command_line(msg)
        self.fire_event("moving", "moving")

        if wait:
            self.wait_for_move_done()

    def move_home(self, speed:int=50, wait:bool=False):
        msg = 'h ' + str(speed) + '\n'
        self.__endMovingEvent.clear()
        self.__send_command_line(msg)
        self.fire_event("moving", "moving_home")

        if wait:
            self.wait_for_move_done()

    def stop_moving(self):
        msg = 'r\n'
        self.__send_command_line(msg)

    def get_position(self):
        msg = 's\n'
        self.__stateRecivedEvent.clear()
        self.__send_command_line(msg)

        if self.__stateRecivedEvent.wait(default_timeout):
            return self.__currentState
        self.logger.error(f"{self._name}: can't get state for too long")
        raise RuntimeError(f"{self._name} can't get state for too long!")

    def set_home_position(self, positions:list):
        if len(positions) != self.__n_servos:
            raise RuntimeError("Wrong number of positions!")

        msg = 'e ' + " ".join(map(str, positions)) + '\n'
        self.__send_command_line(msg)
    
    def wait_for_move_done(self, timeout:float=360.):
        """Blocks current thread until recived end moving message
        from controller or timeout is exceeded

        Args:
            timeout (float, optional): value in seconds. Defaults to 360..

        Raises:
            RuntimeError: if timeout was exceeded
        """
        if not self.__endMovingEvent.wait(timeout):
            raise RuntimeError(f"Haven't reached target in {timeout} s")

    def __send_command_line(self, msg):
        if not self.is_open:
            raise RuntimeError("Device not opened!")
        
        self.__isFreeEvent.wait(default_timeout)
        
        self.__isFreeEvent.clear()
        try:
            self.__commandAcceptedEvent.clear()
            self.logger.info(f"To {self._name}: {msg[:-1]}")
            self.__sp.write(msg.encode("utf-8"))
            self.__sp.flush()
            
            if not self.__commandAcceptedEvent.wait(default_timeout * 0.5):
                self.logger.error(f"{self._name}: command not been recived!")
                raise RuntimeError(f"Can't send command to {self._name}")
        finally:    
            self.__isFreeEvent.set()

        
    def __reading_port_thread_work(self):
        while self.__sp.is_open:
            if self.__sp.in_waiting:
                line = self.__sp.readline().decode("utf-8")[:-1]
                self.logger.info(f"from {self._name}: {line}")
                if line.startswith("OK"):
                    self.__commandAcceptedEvent.set()
                elif line.startswith("END"):
                    self.__endMovingEvent.set()
                    self.fire_event("moving", "done")
                elif line.startswith("N_SERVOS"):
                    self.__n_servos = int(line.split(':')[1])
                else:
                    try:
                        res = line.split('\t')
                        if len(res) == (self.__n_servos + 1):
                            self.__currentState = ([int(s) for s in res[:-1]], float(res[-1]))
                            self.__stateRecivedEvent.set()
                            self.fire_event("position", ([int(s) for s in res[:-1]], float(res[-1])))
                    except Exception:
                        pass
            else:
                time.sleep(0.02)


if __name__ == '__main__':
    logging.basicConfig(filename='test.log', level=logging.INFO, filemode="w")

    s = ArduinoManipulator("TestDevice")
    try:
        s.open_device(0)

        p, d = s.get_position()
        print("sonar value:", d, "cm")
        print("n_servos:", s.n_servos)
        print("current position:", p)

        for i in range(s.n_servos):
            p[i] += 30
            s.move(p, 70, True)
            
            p[i] -= 60
            s.move(p, 70, True)
            p, d = s.get_position()
            print("sonar value:", d, "cm")
            print("current position:", p)
            
        s.move_home(50,True)
    finally:
        s.close_device()
