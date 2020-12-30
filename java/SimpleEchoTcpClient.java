import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;


public class SimpleEchoTcpClient extends SimpleEchoClient {
	Socket socket;

	PrintWriter socketOutputStream;
	BufferedReader socketInputStream;

	public SimpleEchoTcpClient(String host, int port, String message) {
		super(host, port, message);
		try {
			socket = new Socket(host, port);
			socketOutputStream = new PrintWriter(socket.getOutputStream(), true);
			socketInputStream = new BufferedReader(new InputStreamReader(socket.getInputStream()));
		} catch (IOException e) {
			System.err.println("Failed to create client socket");
			throw new RuntimeException(e);
		}
		
	}

	@Override
	protected void sendToServer() {
		socketOutputStream.println(message);
	}

	@Override
	protected String receiveFromServer() {
		try {
			return socketInputStream.readLine();
		} catch (IOException e) {
			System.err.println("Failed to receive response from server");
			throw new RuntimeException(e);
		}
	}

	@Override
	protected void stop() {
		try {
			socketInputStream.close();
			socketOutputStream.close();
			socket.close();
		} catch (IOException e) {
			System.err.println("Failed to close all streams");
			throw new RuntimeException(e);
		}
	}
}
