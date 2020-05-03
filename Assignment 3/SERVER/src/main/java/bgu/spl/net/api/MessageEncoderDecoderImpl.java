package bgu.spl.net.api;

import bgu.spl.net.api.bidi.messagesToClient.Ack;
import bgu.spl.net.api.bidi.messagesToClient.Notification;
import bgu.spl.net.api.bidi.messagesToServer.*;
import bgu.spl.net.api.bidi.messagesToClient.Error;


/* to implement
 */
public class MessageEncoderDecoderImpl implements MessageEncoderDecoder {

    private byte[] messageInProcess = new byte[1<<10];
    private int index = 0;
    private short type;
    private BasicMessageToServer msg;

    /**
     * add the next byte to the decoding process
     *
     * @param nextByte the next byte to consider for the currently decoded
     * message
     * @return a message if this byte completes one or null if it doesnt.
     */
    public Object decodeNextByte(byte nextByte){
        messageInProcess[index] = nextByte;
        index++;
        Object result = null;
        if (index == 2) {
            msg = matchTypeToMessage();
            if (msg instanceof Logout || msg instanceof UserList) {
                result = getResult(nextByte);
            }
        } else if (index > 2) {
            result = getResult(nextByte);
        }
        return result;
    }

    /**
     * encodes the given message to bytes array
     *
     * @param message the message to encode
     * @return the encoded bytes
     */
    public byte[] encode(Object message){
        if (message instanceof Error)
            return ((Error) message).encode();
        else if (message instanceof Notification)
            return ((Notification) message).encode();
        else if (message instanceof Ack)
            return ((Ack)message).encode();
        else
            throw new RuntimeException("message type does not have an encode function");//
    }

    private short bytesToShort ( byte[] byteArr) {
        short result = (short) ((byteArr[0] & 0xff) << 8);
        result += (short) (byteArr[1] & 0xff);
        return result;
    }

    private BasicMessageToServer matchTypeToMessage() {
        type = bytesToShort(messageInProcess);
        if (type == 1)
            return new Register();
        else if (type == 2)
            return new Login();
        else if (type == 3)
            return new Logout();
        else if (type == 4)
            return new Follow();
        else if (type == 5)
            return new Post();
        else if (type == 6)
            return new PM();
        else if (type == 7)
            return new UserList();
        else if (type == 8)
            return new Stat();
        else
            throw new RuntimeException("type does not match any message");
    }

    private Object getResult(byte nextByte) {
        Object result = msg.decode(nextByte);
        if (result == null)
            return null;
        else {// result =! null
            messageInProcess = new byte[1<<10];
            index = 0;
            return result;
        }
    }
}
