#!/usr/bin/env python
from serial.tools import list_ports
str = list_ports.comports()  # Outputs list of available serial ports
print(str)
