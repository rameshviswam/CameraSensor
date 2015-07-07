import java.net.*;
import java.io.*;

public class TCPEchoServer {

	private Socket socket = null;
	private ServerSocket server = null;
	private DataInputStream streamIn = null;
	private DataOutputStream streamOut = null;

	public TCPEchoServer(int port) {
		try {
			System.out
					.println("Binding to port " + port + ", please wait  ...");
			server = new ServerSocket(port);
			System.out.println("Server started: " + server);
			System.out.println("Waiting for a client ...");
			socket = server.accept();
			System.out.println("Client accepted: " + socket);
			open();
			boolean done = false;
			while (!done) {
				try {
					String line = streamIn.readUTF();
					System.out.println(line);
					done = line.equals("bye");

					streamOut.writeUTF(line);
				} catch (IOException ioe) {
					done = true;
				}
			}
			close();
		} catch (IOException ioe) {
			System.out.println(ioe);
		}
	}

	public void open() throws IOException {
		streamIn = new DataInputStream(new BufferedInputStream(
				socket.getInputStream()));
		streamOut = new DataOutputStream(socket.getOutputStream());
	}

	public void close() throws IOException {
		if (socket != null)
			socket.close();
		if (streamIn != null)
			streamIn.close();
		if (streamOut != null)
			streamOut.close();
	}

	public static void main(String args[]) {
		TCPEchoServer server = null;
		if (args.length != 1)
			System.out.println("Usage: java EchoServer port");
		else
			server = new TCPEchoServer(Integer.parseInt(args[0]));
	}
}