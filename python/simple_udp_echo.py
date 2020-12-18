"""Module with simple UDP server and client.
"""
import argparse
import socket
import sys

ENCODING = 'utf-8'

def run_server(host: str, port: int):
	with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:

		s.bind((host, port))

		while True:
			print('Waiting to receive data')
			msg, addr = s.recvfrom(1024)
			print(f'received {msg} from {addr}')
			s.sendto(msg.decode(ENCODING).upper().encode(ENCODING),
				addr)
			

def run_client(host: str, port: int, msg: str):
	with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
		print(f'Sending message {msg} to {host}:{port}')
		s.sendto(msg[0:1024].encode(ENCODING), (host, port))
		reply, addr = s.recvfrom(1024)
		print(f'Received reply {reply} from {addr}')


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
