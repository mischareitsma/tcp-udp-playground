import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class SimpleEchoTcpServer extends SimpleEchoServer {
	
	ServerSocket socket;
	Socket clientSocket;
	PrintWriter clientSocketOutputStream;
	BufferedReader clientSocketInputStream;
	
	
	public SimpleEchoTcpServer(String host, int port) {
		super(host, port);
		try {
			socket = new ServerSocket(port);
		} catch (IOException e) {
			System.err.println("Failed to create server socket");
			throw new RuntimeException(e);
		}
	}

	@Override
	protected String receiveFromClient() {
		try {
			clientSocket = socket.accept();
			clientSocketInputStream = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
			return clientSocketInputStream.readLine();
		} catch (IOException e) {
			System.err.println("Failed to deal with client socket");
			try {
				if (clientSocket != null) {
					clientSocket.close();
				}
				if (clientSocketInputStream != null) {
					clientSocketInputStream.close();
				}
			} catch (IOException re) {
				System.err.println("Failed to close streams during error handling");
			}

		}
		return null;
	}

	@Override
	protected void sendToClient(String message) {

		try {
			clientSocketOutputStream = new PrintWriter(clientSocket.getOutputStream(), true);
			clientSocketOutputStream.println(message);

			clientSocket.close();
			clientSocketInputStream.close();
			clientSocketOutputStream.close();
		} catch (IOException e) {
			System.err.println("Something failed while sending or closing streams");
		}
	}

	@Override
	protected void stop() {
		try {
			socket.close();
		} catch (IOException e) {
			System.err.println("Failed to close server socket");
			throw new RuntimeException(e);
		}
	}
}
