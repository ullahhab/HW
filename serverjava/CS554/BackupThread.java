import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;

public class BackupThread extends Thread {
		private Socket socket;
		private Server server;
		private Identity ID;
		private String response;
		private String[] parse;
		private String msg;
		private int value;
		private Socket sendSocket;
		
		public BackupThread(Socket socket, Server server, Identity ID) {
			// TODO Auto-generated constructor stub
			this.socket = socket;
			this.server = server;
			this.ID = ID;
			//send the join and get the response.
		}
	
	public void run() {
		try {
			InputStream input = socket.getInputStream();
			BufferedReader reader = new BufferedReader(new InputStreamReader(input));
			
			OutputStream output = socket.getOutputStream();
			PrintWriter writer = new PrintWriter(output, true);
			
			///there would be an if statement to see if the connection is client or server.
			do {
				response = reader.readLine();
				parse = response.split(":");
				if(parse.length>1) {
					value = Integer.parseInt(parse[1]);
				}
					if(msg.equals("UPDATE")) {
						server.read = value;
						writer.println("COMPLETE_UPDATE");
						System.out.println("DEBUG9 Update to this server is complete");
					}
				else {
					writer.println("invalid response try again");
				}
			//System quits if msg is exit;
			} while(!msg.equals("quit"));
			socket.close();
}
	catch(IOException ex) {
		System.out.println("Server Error: "+ex.getMessage());
		ex.printStackTrace();
	}
		
	
}
}

