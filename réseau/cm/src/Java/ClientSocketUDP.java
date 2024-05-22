import java.io.*;
import java.net.*;

public class ClientSocketUDP {
    public static void main(String[] args) throws IOException {
        if (args.length != 1) {
            System.out.println("Usage: java ClientSocketUDP message");
            return;
        }
        DatagramSocket socket = new DatagramSocket();
        InetAddress address = InetAddress.getByName("localhost");
        
        byte[] buf = args[0].getBytes();
        DatagramPacket packet = new DatagramPacket(buf, buf.length, address, 4445);
        socket.send(packet);
        
        byte[] resp = new byte[256];
        packet = new DatagramPacket(resp, resp.length);
        socket.receive(packet);

        String received = new String(packet.getData(), 0, packet.getLength());
        System.out.println("Echo: " + received);
    
        socket.close();
    }
}
