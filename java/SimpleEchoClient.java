public class SimpleEchoClient extends SimpleEcho {

	protected String message;

	public SimpleEchoClient(String host, int port, String message) {
		super(host, port);
		this.message = message;
	}
}
