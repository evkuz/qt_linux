import socket
import cv2
import numpy as np


class ImageClient:
	
	def __init__(self, addr, port):
		self.addr = addr
		self.port = port
		
	
	def get_frame(self):
		s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		s.connect((self.addr, self.port))
		
		dataLength = int.from_bytes(s.recv(4), "big")
		print("Data size:", dataLength)
		data = bytearray()
		while len(data) < dataLength:
			part = s.recv(dataLength - len(data))
			if not part: break
			data.extend(bytearray(part))
		s.close()
		
		data = np.asarray(data, dtype="uint8")
		frame = cv2.imdecode(data, cv2.IMREAD_ANYCOLOR)
		return frame

