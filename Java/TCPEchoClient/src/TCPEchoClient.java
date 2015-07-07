import java.net.*;
import java.io.*;

public class TCPEchoClient {

	private Socket socket = null;
	private DataInputStream console = null;
	private DataInputStream streamIn = null;
	private DataOutputStream streamOut = null;

	public TCPEchoClient(String serverName, int serverPort) {
		System.out.println("Establishing connection. Please wait ...");
		try {
			socket = new Socket(serverName, serverPort);
			System.out.println("Connected: " + socket);
			start();
		} catch (UnknownHostException uhe) {
			System.out.println("Host unknown: " + uhe.getMessage());
		} catch (IOException ioe) {
			System.out.println("Unexpected exception: " + ioe.getMessage());
		}
		String line = "";
		while (!line.equals("bye")) {
			try {
				line = console.readLine();
				line += "\r\n";
				streamOut.writeUTF(line);
				System.out.println("server says:" + streamIn.readUTF());
			} catch (IOException ioe) {
				System.out.println("Sending error: " + ioe.getMessage());
			}
		}
	}

	public void start() throws IOException {
		console = new DataInputStream(System.in);
		streamIn = new DataInputStream(socket.getInputStream());
		streamOut = new DataOutputStream(socket.getOutputStream());
	}

	public void stop() {
		try {
			if (console != null)
				console.close();
			if (streamOut != null)
				streamOut.close();
			if (socket != null)
				socket.close();
			if (streamIn != null)
				streamIn.close();
		} catch (IOException ioe) {
			System.out.println("Error closing ...");
		}
	}

	public static void main(String args[]) {
		TCPEchoClient client = null;
		if (args.length != 2)
			System.out.println("Usage: java ChatClient host port");
		else
			client = new TCPEchoClient(args[0], Integer.parseInt(args[1]));
	}
}