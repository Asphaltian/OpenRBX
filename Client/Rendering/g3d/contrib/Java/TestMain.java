import com.graphics3d.g3d.*;
import java.math.*;
import java.io.*;
import Test.*;

/**
 <PRE>

  javac -cp . com/graphics3d/g3d/*.java
  javac -cp . Test/*.java
  javac -cp . TestMain.java

  java -cp . -ea TestMain TestMain
</PRE>
 */
class TestMain {

    public static void main(String[] args) {
        boolean assertionsOn = false;
        assert assertionsOn = true;
        if (! assertionsOn) {
            System.out.println("Must run TestMain with 'java -ea TestMain'.");
            System.exit(-1);
        }

        TestReliableConduit.test();
        TestBinaryIO.test();

        System.out.println("All tests passed.");
    }
    
}
