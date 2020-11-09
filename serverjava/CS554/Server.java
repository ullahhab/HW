//package CS554;

import java.io.*;
import java.net.*;
import java.util.Date;

public class Server {
	//socket connection array count
	int count=0;
	//socket array[]
	Socket[] socArray = new Socket[100000];
	boolean is_primary;
	int port;
	int primPort;
	int read = 0;
	int Back[];
	Socket primSocket;
	//Backup server counter only used by Primary Server
	int backSer=0;
	//Primary port constructor
	public Server(int primary_port){
		this.port = primary_port;
		this.is_primary = true;
		this.primPort = primary_port;
		}
	//Backup port constructor need primary server socket 
	public Server(int port1, int port2) {
		this.port = port1;
		this.primPort = port2;
		this.is_primary=false;
	}
	//If primary port retruns true otherwise it will return false
	public boolean isPrimary(){
		return is_primary;
	}
	//Returns Port number
	public int getPort() {
		return port;
	}
	
	//Read function for the server
	public void Read(Socket socket) {
		OutputStream output;
		try {
			output = socket.getOutputStream();
			PrintWriter writer = new PrintWriter(output,true);
			writer.println(read);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
	}
	//writes the value
	public void Write(int write,Socket socket) {
		read = write;
		OutputStream output;
		try {
			output = socket.getOutputStream();
			PrintWriter writer = new PrintWriter(output, true);
			for(int i=0;i<this.count; i++) {
				//TODO: Request for update should not be send to the backup server requesting.
				Socket soc = this.socArray[i];
				OutputStream output1 = soc.getOutputStream();
				PrintWriter writer1 = new PrintWriter(output1,true);
				InputStream input1 = soc.getInputStream();
				BufferedReader reader = new BufferedReader(new InputStreamReader(input1)); 
				while(reader.readLine().equals("COMPLETE_UPDATE")) {
					writer1.println("UPDATE:"+write);
				}
				
			}
			writer.println("COMPLETE_WRITE");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	//Joins the backup server 
	//TODO: Save the socket information in the array of socket instead of .[DONE]
	public void Join(Socket socket) {
		saveSocket(socket);
		try{
			OutputStream output = socket.getOutputStream();
			PrintWriter writer = new PrintWriter(output,true);
			writer.println("JOIN_COMPLETE");
			System.out.println("JUST SENT JOIN_COMPLETE");

		} catch(IOException e) {
			e.printStackTrace();
		}
		
		
	}
	//updates the values
	public void Update(Socket socket,String msg) {
		//sent the message to the requesting server
		try {
			OutputStream output = socket.getOutputStream();
			PrintWriter writer = new PrintWriter(output, true);
			writer.println(msg);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	//This method and remove socket are only used by primary server
	public void saveSocket(Socket socket) {
		System.out.println("DEBUG "+socket);
		this.socArray[count] = socket;
		System.out.println("DEBUG2 "+socArray[count]); 
		count+=1;
	}
	//removes socket that has just quit and shift all the value back to the normal
	public void removeSocket(Socket socket) {
		for(int i=0; i<count;i++) {
			if(socArray[i].equals(socket)) {
				shiftArray(i);
				
			}
			
		}
		count-=1;
	}
	
	public void shiftArray(int value) {
		if(value<count-1) {
			for(int i=value; i<count-1; i++) {
				socArray[i]=socArray[i+1];
			}
		}
	}
	
	


}
