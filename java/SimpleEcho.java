public class SimpleEcho {

	// Just protected fields, encapsulation is overkill for simple examples
	protected String host;
	protected int port;

	public SimpleEcho(String host, int port) {
		this.host = host;
		this.port = port;
	}
	
	protected enum SocketType {
		TCP, UDP;
	}

	protected enum ClientServer {
		CLIENT, SERVER;
	}

	public static void main(String[] args) {
		System.out.println("Simple Echo Start");

		SimpleEcho se = SimpleEcho.newInstance(args);
		se.run();

		System.out.println("Simple Echo Stop");
	}

	public static SimpleEcho newInstance(String[] args) {
		Options options = parseArguments(args);

		if (options.socketType == SocketType.TCP) {
			if (options.clientServer == ClientServer.CLIENT) {
				// TODO: Return correct type later when build
				return new SimpleEcho(options.host, options.port);
			}
			else {
				return new SimpleEcho(options.host, options.port);
			}
		}
		else {
			if (options.clientServer == ClientServer.CLIENT) {
				return new SimpleEcho(options.host, options.port);
			}
			else {
				return new SimpleEcho(options.host, options.port);
			}
		}
	}

	protected void run() {
		System.out.println("SimpleEcho.run()");;
	}


	public static Options parseArguments(String[] args) {
		// Arguments should always be --option=arg or --option.

		Options options = new Options();

		boolean isServer = false;
		boolean isClient = false;

		boolean isTcp = false;
		boolean isUdp = false;

		for (String option: args) {
			String[] parts = option.split("=");

			switch(parts[0]) {
				case "--server":
					isServer = true;
					break;
				case "--client":
					isClient = true;
					break;
				case "--tcp":
					isTcp = true;
					break;
				case "--udp":
					isUdp = true;
					break;
				case "--host":
					options.host = parts[1];
					break;
				case "--port":
					options.port = Integer.parseInt(parts[1]);
					break;
				case "--message":
					options.message = parts[1];
					break;
				default:
					System.err.println("Invalid options " + option);
					break;
			}
		}

		if (isServer == isClient) {
			throw new RuntimeException("Cannot be client and server, or neither");
		}

		if (isTcp == isUdp) {
			throw new RuntimeException("Cannot be TCP and UDP, or neither");
		}

		options.clientServer = isServer ? ClientServer.SERVER : ClientServer.CLIENT;
		options.socketType = isTcp ? SocketType.TCP : SocketType.UDP;

		return options;

	}
}

class Options {
	SimpleEcho.SocketType socketType;
	SimpleEcho.ClientServer clientServer;
	String host;
	int port;
	String message;
}
