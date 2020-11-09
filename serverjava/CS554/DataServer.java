//package CS554;

import java.io.*;
import java.net.*;

/**
 * @author secondary
 *
 */
public class DataServer {
	


	/**
	 * @param args
	 * @throws IOException 
	 * @throws NumberFormatException 
	 */
	public static void main(String[] args){
		// TODO Auto-generated method stub
		Identity ID;
		if(args.length<1 || args.length>2) {
			System.out.println("Invalid numeber of arguments: Usage: java DataServer port1 port2 (for back up server) and java DataServer port (for primary server)");
			return;
		}
		if(args.length == 1) {
			Server server = new Server(Integer.parseInt(args[0]));
		
			//Start the socket and run as the primary server 
			try(ServerSocket serverSocket = new ServerSocket(Integer.parseInt(args[0]))) {
				System.out.println("I'm the primary server:");
				System.out.println("Data server is listening on: "+server.getPort());
				ID = Identity.primary;
				while(true) {
					Socket socket = serverSocket.accept();
					new ServerThread(socket, server, ID).start();
				}
				
			}
			catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

			
			
			
			
		}
		else if(args.length ==2) {
			Server server = new Server(Integer.parseInt(args[0]), Integer.parseInt(args[1]));
			//Start the socket and run as the Backup server
			try(ServerSocket serverSocket = new ServerSocket(Integer.parseInt(args[0]))) {
				System.out.println("I'm the Backup server:");
				System.out.println("Primary port: "+Integer.parseInt(args[1]));
				System.out.println("Data server is listening on: "+server.getPort());
				//create new socket that is with primary 
				Socket primSocket = new Socket("localhost",Integer.parseInt(args[1]));
				InputStream input = primSocket.getInputStream();
				BufferedReader reader = new BufferedReader(new InputStreamReader(input));
				OutputStream output = primSocket.getOutputStream();
				PrintWriter writer = new PrintWriter(output,true);
				writer.println("JOIN:"+Integer.parseInt(args[1]));
				System.out.println("Just send out: JOIN:"+Integer.parseInt(args[1])+" to port: "+Integer.parseInt(args[1]));
				while(reader.readLine().equals("JOIN_COMPLETE")) {
					System.out.println("DEBUG3 Response got "+reader.readLine());
					continue;
				}
				System.out.println("DEBUG 4 JOINED SUCCESS");
				ID = Identity.backup;
				while(true) {
					Socket socket = serverSocket.accept();
					new ServerThread(socket, server, ID,primSocket).start();
				}
		}
			catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
}

