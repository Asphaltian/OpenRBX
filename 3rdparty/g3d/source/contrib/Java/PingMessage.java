
import com.graphics3d.g3d.*;

public class PingMessage implements BinarySerializable, BinaryDeserializable {

    public String text;

    public PingMessage() {
        text = "";
    }
    public PingMessage(String s) {
        text = s;
    }

    public void serialize(BinaryOutput b) {
        b.writeString(text);
    }
 
    public void deserialize(BinaryInput b) {
        text = b.readString();
    }
}