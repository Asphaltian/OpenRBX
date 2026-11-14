package Test;

import com.graphics3d.g3d.*;
import java.math.*;
import java.io.*;

class Msg implements BinarySerializable, BinaryDeserializable {
    int x;

    public Msg() {
    }

    public Msg(int y) {
        x = y;
    }

    public boolean equals(Msg m) {
        return x == m.x;
    }

    public void deserialize(BinaryInput b) {
        // Make the message long
        x = b.readInt32();
        for (int i = 0; i < 99; ++i) {
            int y = b.readInt32();
            assert x == y;
        }
    }

    public void serialize(BinaryOutput b) {
        for (int i = 0; i < 100; ++i) {
            b.writeInt32(x);
        }
    }
}


public class TestReliableConduit {
    public static void test() {
        System.out.print("ReliableConduit ");

        try {
            int port = 11112;

            NetListener serverListener = 
                new NetListener(port);

            assert ! serverListener.clientWaiting();

            ReliableConduit clientConduit = 
                new ReliableConduit("localhost", port);

            assert serverListener.clientWaiting();
            assert clientConduit.ok();

            ReliableConduit serverConduit = 
                serverListener.waitForConnection();

            assert ! serverListener.clientWaiting();

            assert clientConduit.ok();
            assert serverConduit.ok();

            // Send from client to server
            final int type = 1;
            final int value = 12345;

            Msg m1 = new Msg(value);

            assert ! serverConduit.messageWaiting();
            assert ! clientConduit.messageWaiting();

            clientConduit.send(type, m1);

            assert ! clientConduit.messageWaiting();

            while (! serverConduit.messageWaiting());
            
            Msg m2 = new Msg();
            assert serverConduit.waitingMessageType() == type;
            serverConduit.receive(m2);
            
            // Make sure the message arrived correctly
            assert m2.x == m1.x;

            assert ! serverConduit.messageWaiting();
            assert ! clientConduit.messageWaiting();
            
            // Send from server to client
            serverConduit.send(type, m2);
            clientConduit.receive(m1);
            assert m2.x == m1.x;

            assert ! serverConduit.messageWaiting();
            assert ! clientConduit.messageWaiting();

        } catch (IOException e) {
            System.err.println(e);
            System.exit(-1);
        }

        System.out.println("passed");
    }

}
