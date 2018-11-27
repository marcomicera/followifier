/**
 * EchoUpperServer
 * 
 * This server accepts in input any kind of string and
 * replies with their case-toggled version.
 * For instance, sending "hEllo" will cause this server
 * to reply with "HeLLO".
 * 
 * Communication with this server can be achieved by means
 * of tools like netcat or telnet.
 */

package it.polito.pds.followifier;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;
import java.util.stream.Collectors;

public class EchoUpperServer {
	/**
	 * Server info.
	 */
	private static final String SERVER_NAME = "EchoUpperServer";
	private static final int DEFAULT_SERVER_PORT = 9991; 

	/**
	 * Keyword to be sent in order to close the connection with the server.
	 */
	private static final String END_CONNECTION_KEYWORD = "quit";
	
	/**
	 * Message displayed upon starting this server.
	 */
	private static final String WELCOME_MESSAGE = "Successfully connected to " + SERVER_NAME + ": enter '"
			+ END_CONNECTION_KEYWORD + "' to exit.";

	/**
	 * Main method
	 * @param arg	this server's listening port number
	 */
	public static void main(String[] args) {
		// This server's listening port number
		int server_port;
				
		// Arguments number and format checking
		if(args.length != 1 || !isInteger(args[0]))
			// Using the default server port number
			server_port = DEFAULT_SERVER_PORT;
		else
			// Using the server port number specified by the user as argument
			server_port = Integer.parseInt(args[0]);
		
		// Starting the server
		startServer(server_port);
		
		System.out.println(SERVER_NAME + " shutted down.");
	}

	/**
	 * It starts the server on a specified port.
	 * @param port	this server's listening port number.
	 */
	public static void startServer(int port) {
		// Creating a server socket
		try (ServerSocket serverSocket = new ServerSocket(port, 7, InetAddress.getLocalHost())) {
			System.out.println(SERVER_NAME + " started, " + serverSocket);
			
			// Cyclic server
			while(true) {
				// Accepting a client's request
				Socket connectionSocket = serverSocket.accept();
				System.out.println(
					"A new connection has been accepted from " 
					+ connectionSocket.getRemoteSocketAddress()
				);
				
				// Creating a request handler thread
				Thread requestHandler = new Thread() {
					@Override
					public void run() {
						// Handling the request
						handleRequest(connectionSocket);
					}
				};

				// Starting the thread that will handle this request
				requestHandler.start();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * It handles a client's request by responding with case-toggled strings.
	 * @param connectionSocket		the socket by which the handler can reply.
	 */
	private static void handleRequest(Socket connectionSocket) {
		try {
			// Input and output streams for this connection
			InputStream inputToServer = connectionSocket.getInputStream();
			OutputStream outputFromServer = connectionSocket.getOutputStream();
			System.out.println("Streams created for " + connectionSocket.getRemoteSocketAddress());
	
			// New scanner for reading lines
			try (Scanner scanner = new Scanner(inputToServer, "UTF-8")) {
				PrintWriter serverPrintOut = new PrintWriter(new OutputStreamWriter(outputFromServer, "UTF-8"), true);
	
				// Displaying a welcome messages
				serverPrintOut.println(WELCOME_MESSAGE);
				System.out.println("Replied with a welcome message to " + connectionSocket.getRemoteSocketAddress());
				
				// Replying until the end connection keyword is received
				boolean run = true;
				while (run && scanner.hasNextLine()) {
					// Retrieving a new string line
					String line = scanner.nextLine();
					
					// Computing the response
					String response = toggleString(line);
					
					// Displaying info on this server
					System.out.println(
						"New line received from " 
						+ connectionSocket.getRemoteSocketAddress() 
						+ ": " + line + "; "
						+ "replied with: " + response
					);
					
					// Replying with the toggled-case version
					serverPrintOut.println(SERVER_NAME + " replied: " + response);
	
					// Checking whether the end connection keyword has been received
					if (line.toLowerCase().trim().equalsIgnoreCase(END_CONNECTION_KEYWORD))
						run = false;
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * Given a string, it returns its case-toggled version.
	 * @param sentence	the string to be toggled.
	 * @return			the case-toggled version.
	 */
	private static String toggleString(String sentence) {
		return sentence.chars().mapToObj(c -> {
			if (Character.isUpperCase(c)) {
				c = Character.toLowerCase(c);
			} else if (Character.isLowerCase(c)) {
				c = Character.toUpperCase(c);
			}
			return String.valueOf((char) c);
		}).collect(Collectors.joining());
	}
	
	/**
	 * It checks whether a string represents an integer or not.
	 * @param string	string to be checked.
	 * @return			true when the string represents an integner.
	 * 					false otherwise.
	 */
	private static boolean isInteger(String string) {
	    try { 
	    	// Trying to parse this string into an integer
	        Integer.parseInt(string); 
	    } catch(NumberFormatException e) { 
	        return false; 
	    } catch(NullPointerException e) {
	        return false;
	    }

	    // No exceptions thrown: this string represents an integer 
	    return true;
	}
}