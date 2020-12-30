import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class SimpleEchoUdpServer extends SimpleEchoServer {

	DatagramSocket socket;
	InetAddress clientAddress;
	int clientPort;

	// byte[] buffer = new byte[1024];

	public SimpleEchoUdpServer(String host, int port) {
		super(host, port);

		try {
			socket = new DatagramSocket(port);
		} catch (IOException e) {
			System.err.println("Failed to create server socket");
			throw new RuntimeException(e);
		}
	}

	@Override
	protected String receiveFromClient() {
		try {
			byte[] buffer = new byte[1024];
;			DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
			socket.receive(packet);
			clientAddress = packet.getAddress();
			clientPort = packet.getPort();
			return new String(packet.getData(), 0, packet.getLength());

		} catch (IOException e) {
			System.err.println("Failed to receive packet");
		}

		return null;
	}

	@Override
	protected void sendToClient(String message) {
		try {
			byte[] buffer = message.getBytes();
			DatagramPacket packet = new DatagramPacket(buffer, buffer.length, clientAddress, clientPort);
			socket.send(packet);
		} catch (IOException e) {
			System.err.println("Failed to send repsonse to client");
		}
	}

	@Override
	protected void stop() {
		socket.close();
	}
	
}
