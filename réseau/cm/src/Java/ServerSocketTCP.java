import java.net.*;
import java.io.*;

public class ServerSocketTCP {
    public static void main(String[] args) {
        ServerSocket listeningSocket = null;
        
        try {
            listeningSocket = new ServerSocket(44400);
        } catch (IOException e) {
            System.err.println("Could not listen on port 44400");
            System.exit(1);
        }
        
        while (true) {
            Socket serviceSocket = null;
            try {
                serviceSocket = listeningSocket.accept();
            } catch (IOException e) {
                System.err.println("I/O error while accept");
                System.exit(1);
            }
            new ProcessConnection(serviceSocket).start();
        }
    }
}
