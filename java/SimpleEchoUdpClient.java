import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class SimpleEchoUdpClient extends SimpleEchoClient {

	DatagramSocket socket;
	InetAddress inetAddress;
	byte[] buffer;

	public SimpleEchoUdpClient(String host, int port, String message) {
		super(host, port, message);
		buffer = message.getBytes();

		try {
			socket = new DatagramSocket();
			inetAddress = InetAddress.getByName(host);
		} catch (IOException e) {
			System.err.println("Failed to create client socket");
			throw new RuntimeException(e);
		}
	}

	@Override
	protected void sendToServer() {
		try {
			DatagramPacket packet = new DatagramPacket(buffer, buffer.length, inetAddress, port);
			socket.send(packet);
		}
		catch (IOException e) {
			System.err.println("Failed to send datagram to server");
			throw new RuntimeException(e);
		}

	}

	@Override
	protected String receiveFromServer() {
		try {
			DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
			socket.receive(packet);
			String reply = new String(packet.getData(), 0, packet.getLength());
			return reply;
		} catch (IOException e) {
			System.err.println("Failed to receive server reply");
			throw new RuntimeException(e);
		}
	}

	@Override
	protected void stop() {
		socket.close();

	}
	
}
