public abstract class SimpleEchoServer extends SimpleEcho {

	public SimpleEchoServer(String host, int port) {
		super(host, port);
	}

	protected String getReplyMessage(String message) {
		return message.toUpperCase();
	}

	protected abstract String receiveFromClient();
	protected abstract void sendToClient(String message);

	@Override
	protected void run() {
		boolean keepRunning = true;

		while (keepRunning) {
			String message = receiveFromClient();
			System.out.println("Message received from client: " + message);

			String reply = getReplyMessage(message);
			System.out.println("Replying to client: " + reply);
			sendToClient(reply);

			if (message.startsWith("Stop Server")) {
				keepRunning = false;
			}
		}
	}
}
