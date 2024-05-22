import java.io.*;
import java.net.*;

public class ServerSocketUDP {
    public static void main(String[] args) throws IOException {
        DatagramSocket socket = new DatagramSocket(4445);
        
        while (true) {
            byte[] buf = new byte[256];
            DatagramPacket packet = new DatagramPacket(buf, buf.length);
            socket.receive(packet);
            
            InetAddress address = packet.getAddress();
            int port = packet.getPort();
            packet = new DatagramPacket(buf, buf.length, address, port);
            socket.send(packet);
        }
    }
}
