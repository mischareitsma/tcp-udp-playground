public abstract class SimpleEchoClient extends SimpleEcho {

	protected String message;

	public SimpleEchoClient(String host, int port, String message) {
		super(host, port);
		this.message = message;
	}

	@Override
	protected void run() {

		sendToServer();
		
		String reply = receiveFromServer();
		System.out.println("Received from server: " + reply);
	}

	protected abstract void sendToServer();
	protected abstract String receiveFromServer();
}
