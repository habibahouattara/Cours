import java.net.*;
import java.io.*;

public class ProcessConnection extends Thread {
    private Socket socket = null;

    public ProcessConnection(Socket serviceSocket) {
        super("ProcessConnection");
        socket = serviceSocket;
    }
    
    public void run() {
        try {
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            
            String line;
            while ((line = in.readLine()) != null) {
                System.out.println(line);
            }
            in.close();
            socket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
