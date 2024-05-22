import java.io.*;
import java.net.*;

public class ClientSocketTCP {
    public static void main(String[] args) throws IOException {
        Socket socket = null;
        PrintWriter out = null;
        try {
            socket = new Socket("localhost", 44400);
            out = new PrintWriter(socket.getOutputStream());
        } catch (UnknownHostException e) {
            System.err.println("Could not find host");
            System.exit(1);
        } catch (IOException e) {
            System.err.println("I/O Error");
            System.exit(1);
        }

        out.print("bonjour ");
        out.println("tout le monde");
        out.close();
        
        socket.close(); // could throw IOException
    }
}
