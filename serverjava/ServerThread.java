package CS554;

import java.net.Socket;
import java.io.*;


public class ServerThread extends Thread {
	private Socket socket;
	private Server server;
	private Identity ID;
	private String response;
	private String[] parse;
	private String msg;
	private int value;
	private Socket sendSocket;
	

	public ServerThread(Socket socket, Server server, Identity ID) {
		this.socket = socket;
		this.server = server;
		this.ID = ID;
	}
	
	public ServerThread(Socket socket, Server server, Identity ID, Socket primarySocket) {
		// TODO Auto-generated constructor stub
		this.socket = socket;
		this.server = server;
		this.ID = ID;
		server.primSocket = primarySocket;
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
					msg = parse[0];
					switch(ID) {
					case primary:
					
						if(msg.equals("UPDATE")) {
							server.Write(value,socket);
							server.Update(socket,"COMPETE_WRITE");
						}
						else if(msg.equals("WRITE")) {
							server.Write(value,socket);	
						}
						else if(msg.equals("JOIN")) {
							server.Join(socket);
							writer.println("COMPLETE_JOIN");
						}
						
					case backup:
						if(msg.equals("UPDATE")) {
							server.read = value;
							writer.println("COMPLETE_UPDATE");
						}
						else if(msg.equals("WRITE")) {
							server.Update(server.primSocket,"UPDATE");
							writer.println("COMPETE_WRITE");
							} 
					}
					if(msg.equals("READ")) {
						server.Read(socket);
					}
					else {
						writer.println("invalid response try again");
					}
				//System quits if msg is exit;
				} while(!msg.equals("quit"));
	}
		catch(IOException ex) {
			System.out.println("Server Error: "+ex.getMessage());
			ex.printStackTrace();
		}
		
	}
}
