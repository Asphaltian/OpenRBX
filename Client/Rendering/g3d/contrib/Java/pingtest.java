/**
 @file pingtest/main.cpp
 
  pingtest {hostname|ip|--server}

 A simple demo of LightweightConduit and ReliableConduit that
 is also a good first-test for network problems.

 This does not use the protocol of the 'ping' program, but the
 idea is the same (there is no return message from the server,
 however).

 Two protocols are supported, reliable and lightweight.  The reliable
 global variable selects between them.  In each case:  

 1. client opens a Reliable|LightweightConduit
 2. client sends a PingMessage, "hello, server"
 3. server responds with a PingMessage, "hello, client"
 4. client disconnects (reliable only)

 @author Corey Taylor
 @cite ported from Morgan McGuire's C++ pingtest
 */
import com.graphics3d.g3d.*;
import java.net.*;
import java.nio.*;
import java.io.*;

class pingtest {

    static final String clientGreeting = "hello, server";
    static final String serverResponse = "hello, client";

    public static void main(String[] args) {
    
        if (args.length == 0) {
            System.out.println("pingtest machine");
            System.out.println();
            System.out.println("  machine  Host name of the pingtest server");
            //System.out.println("  --server Run a server on this machine");
        } else if (args[0] == "--server") {
            System.out.println("--server not supported");
        } else {
            reliableClient(args[0]);
        }
    }
 
    public static void reliableClient(String server) {

        InetSocketAddress serverAddress = new InetSocketAddress(server, 1201);

        System.out.printf("  Trying to connect to %s\n", serverAddress.toString());

        ReliableConduit conduit = null;
        
        try {
            conduit = new ReliableConduit(serverAddress);
        } catch (Exception e) {
            System.out.printf("\n\nCouldn't create ReliableConduit -- %s\n", e.toString());
            System.exit(-1);
        }

        // Send a hello message
        System.out.printf("  Sending \"%s\"... ", clientGreeting); 
        
        try {
            conduit.send(1008, new PingMessage(clientGreeting));
        } catch (Exception e) {
            System.out.printf("\n\nCouldn't send PingMessage -- %s\n", e.toString());
            System.exit(-1);
        }
        
        System.out.printf("sent.\n");


        // Wait for server response
        System.out.printf("  Waiting for server response");
        
        try {
            while (conduit.messageWaiting() == false) {
                System.out.printf(".");
                try {
                    Thread.sleep(1);
                } catch (InterruptedException e) {
                    System.out.printf("Couldn't sleep\n");
                }
            }
        } catch (Exception e) {
            System.out.printf("\n\nError waiting for response -- %s\n", e.toString());
            System.exit(-1);
        }
        
        System.out.printf("got it.\n");


        PingMessage response = new PingMessage();
        try {
            assert conduit.waitingMessageType() == 1008;
            conduit.receive(response);
        } catch (Exception e) {
            System.out.printf("\n\nCouldn't receive PingMessage -- %s\n", e.toString());
            System.exit(-1);
        }
        
        System.out.printf("  Server responded: \"%s\".\n", response.text);
        
        System.out.printf("  Dropping connection.\n");
    } 
}    
