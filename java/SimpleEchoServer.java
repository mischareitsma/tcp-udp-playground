public class SimpleEchoServer extends SimpleEcho {

	public SimpleEchoServer(String host, int port) {
		super(host, port);
	}

	protected String getReplyMessage(String message) {
		return message.toUpperCase();
	}

}
