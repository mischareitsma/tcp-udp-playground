"""Simple TCP/UDP ping/echo
"""
import argparse
import datetime
import enum
import logging
import socket
import sys

logging.basicConfig(
	format='%(asctime)s - %(levelname)s - %(message)s',
	level=logging.DEBUG
)

# Constants and Enums

# Length should always fit in one byte for now
MAX_MSG_LENGTH = 2**8

class ServerClient(enum.Enum):
	SERVER = 1
	CLIENT = 2

class TransportMechanism(enum.Enum):
	TPC = 1
	UDP = 2

class MaxMsgLengthException(Exception):
	pass

class InvalidHostOrPortException(Exception):
	pass

class UnsupportedException(Exception):
	pass

# Message handlers

class MessageHandler:

	def __init__(self, encoding='utf-8'):
		self.encoding = encoding
	
	def get_reply(self, request: bytes) -> bytes:
		return b''
	
	def get_request(self, request_input: bytes) -> bytes:
		return request_input

class EchoMessage(MessageHandler):
	
	def get_reply(self, request):
		reply = request.decode(self.encoding).upper()
		return reply.encode(self.encoding)

class PingMessage(MessageHandler):

	def get_reply(self, request):
		reply = f'Pong: {datetime.datetime.now().isoformat()}'
		return reply.encode(self.encoding)

	def get_request(self, request_input: bytes) -> bytes:
		request = f'Ping: {datetime.datetime.now().isoformat()}'
		return request.encode(self.encoding)

# Socket Transport Class
# TODO: Maybe a better hierarchy: SocketTransport -> Socket{Client,Server} -> Socket{Tcp,Udp}{Server,Client}

class SocketTransport:

	def __init__(self, server_or_client: ServerClient, 
		transport_mechanism: TransportMechanism, host: str, port: int):

		self.server_or_client: ServerClient = server_or_client
		self.transport_mechanism: TransportMechanism = \
			transport_mechanism

		logging.debug('Constructing SocketTransport instance')
		logging.debug(f'\tServer or Client: {server_or_client}')
		logging.debug(f'\tTransport Mechanism: {transport_mechanism}')

		self.host: str = None
		self.port: int = None

		self.set_host_and_port(host, port)

		self.encoding = 'utf-8'

		self.socket = None

		self.msg_handler: MessageHandler = None

	def get_msg(self, msg: bytes):
		msg_len = len(msg)

		if msg_len > MAX_MSG_LENGTH:
			raise MaxMsgLengthException(f'Message length {msg_len}'
				f' exceeds max of {MAX_MSG_LENGTH}')

		return bytes([msg_len]) + msg

	def set_host_and_port(self, host: str, port: int):
		if host == '':
			raise InvalidHostOrPortException('Missing host or IP')
		
		if not (0 < port <= 65535 ):
			raise InvalidHostOrPortException(
				f'Invalid port value {port}')

		logging.debug(f'\tHost: {host}')
		logging.debug(f'\tPort: {port}')

		self.host = host
		self.port = port

	def run_server(self):
		if not (self.host and self.port):
			raise InvalidHostOrPortException(
				'Missing host and/or port information')

		# Only support TCP for now, but this should change to
		# if self.tcp then do this, else do this.
		if self.transport_mechanism is TransportMechanism.UDP:
			raise UnsupportedException('No support of UDP')

		if self.transport_mechanism is TransportMechanism.TCP:
			self._run_tcp_server()
	
	def _run_tcp_server(self):
		"""Running a TCP server.
		"""
		logging.info('Starting TCP server')

		with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
			s.bind((self.host, self.port))

			while True:
				self._tcp_listen(s)


	def _tcp_listen(self, s: socket.socket):

		logging.info('Waiting for client TCP connections')
		c, a = s.accept()

		logging.info(f'Connection accepted from {a}')

		with c:
			msg_length = ord(c.recv(1))
			logging.debug(f'Length of client request: {msg_length}')
			req = c.recv(msg_length)
			logging.debug(f'Client request: {req}')
			reply = self.msg_handler.get_reply(req)

			logging.debug(f'Response to client: {reply}')

			c.sendall(self.get_msg(reply))

	def client_send(self, msg: bytes) -> bytes:
		if self.transport_mechanism is TransportMechanism.UDP:
			raise UnsupportedException('No support of UDP')

		if self.transport_mechanism is TransportMechanism.TCP:
			return self._client_tcp_send(msg)

	def _client_tcp_send(self, msg:bytes) -> bytes:
		logging.info(f'Sending {msg} to TCP server')

		with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
			s.connect((self.host, self.port))
			s.sendall(self.get_msg(msg))
			reply_len = ord(s.recv(1))
			logging.debug(f'Received reply of length {reply_len}')
			reply = s.recv()
			logging.info(f'Server reply: {reply}')


if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('--client', help='Run as client',
		action='store_true')
	parser.add_argument('--server', help='Run as server',
		action='store_true')

	parser.add_argument('--tcp', help='Use TCP as transport',
		action='store_true')
	parser.add_argument('--udp', help='Use UDP as transport',
		action='store_true')

	parser.add_argument('--port', '-p', help='Port number', type=int,
		dest='port', required=True)
	parser.add_argument('--host', '-h', help='Host name or IP', type=str,
		dest='host', required=True)

	parser.add_argument('--echo', '-e', help='Echo a message', type=str,
		required=False)

	parser.add_argument('--ping', help='Ping a server',
		required=False)

	args = parser.parse_args()

	if args.client == args.server:
		print('Cannot run both as --client and --server')
		sys.exit(1)

	if not (args.client or args.server):
		print('Require --client or --server')
		sys.exit(1)

	if args.tcp == args.udp:
		print('Cannot run both as --tcp and --udp')
		sys.exit(1)

	if not (args.tcp or args.udp):
		print('Require --tcp or --udp')

	cs: ServerClient = None
	tm: TransportMechanism = None

	if args.client:
		cs = ServerClient.CLIENT
	if args.server:
		cs = ServerClient.SERVER
		
	if args.tcp:
		tm = TransportMechanism.TCP
	if args.udp:
		tm = TransportMechanism.UDP
