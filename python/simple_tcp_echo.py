"""Module with simple TCP server and client.
"""
import argparse
import socket
import sys

ENCODING = 'utf-8'

def run_server(host: str, port: int):
	with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
		s.bind((host, port))
		s.listen()

		while True:
			print(f'Waiting for a new connection on {host}:{port}')
			c, a = s.accept()

			print(f'Accepted connection from {a}')

			with c:
				msg = c.recv(1024)
				print(f'Received message: ' +
					msg.decode(ENCODING))
				
				# shout back
				c.sendall(msg.decode(ENCODING).upper() \
					.encode(ENCODING))

def run_client(host: str, port: int, msg: str):
	with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
		print(f'Connecting to server {host}:{port}')
		s.connect((host, port))
		print(f'Sending message: {msg}')
		s.sendall(msg[0:1024].encode(ENCODING))
		reply = s.recv(1024).decode(ENCODING)
		print(f'Received reply: {reply}')

if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('--server', '-s', dest='server',
		action='store_true', help='Run TCP server')
	parser.add_argument('--client', '-c', dest='client',
		action='store_true', help='Run TCP client')
	
	parser.add_argument('--host', dest='host', required=True,
		type=str, help='Connect to host')
	parser.add_argument('--port', dest='port', required=True,
		type=int, help='Connect to port')
	
	parser.add_argument('--message', '-m', dest='message', required=False,
		type=str, default='Some message', help='Message to echo')


	args = parser.parse_args()

	# Cannot be both server and client, and cannot be neither
	if args.server == args.client:
		print('Cannot be both or neither a server and client')
		sys.exit(1)

	if args.server:
		run_server(args.host, args.port)

	if args.client:
		run_client(args.host, args.port, args.message)
