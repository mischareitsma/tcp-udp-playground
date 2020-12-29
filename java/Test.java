public class Test {
	public static void main(String[] args) {
		String[] strings = {
			"--client", "--server", "--host=someHost", "--port=1234"
		};

		for (String opt: strings) {
			System.out.println("Full option string: " + opt);

			for (String part: opt.split("=")) {
				System.out.println("Part: " + part);
			}

			System.out.println("");
		}


	}
}
