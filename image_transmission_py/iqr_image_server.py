import socket
import cv2
from iqr_camera import IQRCamera


class ImageServer:
	
	def __init__(self, addr, port, camera:IQRCamera):
		self.addr = addr
		self.port = port
		self.camera = camera
		self.socket = None
	
	
	def stop(self):
		self.isWorking = False
		if self.socket is not None:
			self.socket.shutdown(socket.SHUT_RDWR)


	def start(self):
		self.isWorking = True
		self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.socket.bind((self.addr, self.port))
		self.socket.listen(3)
		print('Server up')

		while self.isWorking:
			try:
				conn, client_addr = self.socket.accept()
				print('\nClient is connected:', client_addr)
			except Exception as e:
				print("Error while accepting new client. Error:", e)
				continue
			
			try:
				frame = self.camera.get_frame()
				data = cv2.imencode('.jpg', frame)[1].tobytes()
				print("Compessed data size:", len(data))

				data = len(data).to_bytes(4,"big") + data
				try:
					res = conn.send(data)
					if res is None:
						print("Sending of frame was failed!")
					print(f'Frame was succesfully send ({res} bytes)')
				except Exception as e:
					print("Sending of frame was failed! Error:", e)
				
			except Exception as e:
				print("Can't get frame. Error:", e)

			conn.close()
			
		self.socket.close()
		print('Server down')
