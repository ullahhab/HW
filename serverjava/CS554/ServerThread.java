//package CS554;

import java.net.Socket;
import java.io.*;
import java.net.*;

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
	
	public ServerThread(Socket socket, Server server, Identity ID, int port) {
		// TODO Auto-generated constructor stub
		this.socket = socket;
		this.server = server;
		this.ID = ID;
		server.primPort = port;
		//send the join and get the response.
	}

	public void run() {
		try {
				InputStream input = socket.getInputStream();
				BufferedReader reader = new BufferedReader(new InputStreamReader(input));
				
				OutputStream output = socket.getOutputStream();
				PrintWriter writer = new PrintWriter(output, true);
				
				///there would be an if statement to see if the connection is client or server.
					//System.out.println("DEBUG5 "+reader.readLine());
					response = reader.readLine();
					System.out.println("DEBUG5 "+response);
					parse = response.split(":");
					
					if(parse.length>1) {
						value = Integer.parseInt(parse[1]);
						System.out.println("DEBUG6 "+value);
					}
					msg = parse[0];
					System.out.println("DEBUG7 "+msg);
					switch(ID) {
					case primary:
					
						if(msg.equals("UPDATE")) {
							server.Write(value,socket);
							writer.println("COMPLETE_WRITE");
						}
						else if(msg.equals("WRITE")) {
							server.Write(value,socket);	
						}
						else if(msg.equals("JOIN")) {
							server.Join(value,socket);
							writer.println("COMPLETE_JOIN");
						}
						
					case backup:
						if(msg.equals("WRITE")) {
							server.Update(server.primPort,"UPDATE:"+value);
							writer.println("COMPETE_WRITE");
							}
						else if(msg.equals("UPDATE")){
							server.UpdateRead(value);
							writer.println("COMPLETE_UPDATE");
						} 
					}
					if(msg.equals("READ")) {
						server.Read(socket);
					}
					else{
						writer.println("invalid response try again");
					}
				//System quits if msg is exit;
				socket.close();
	}
		catch(IOException ex) {
			System.out.println("Server Error: "+ex.getMessage());
			ex.printStackTrace();
		}
		
	}
}
