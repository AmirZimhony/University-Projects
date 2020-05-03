package bgu.spl.net.api.bidi.messagesToServer;
import bgu.spl.net.api.bidi.ConnectionsImpl;
import bgu.spl.net.api.bidi.myBidiMessagingProtocol;


public abstract class BasicMessageToServer {


    public BasicMessageToServer(){}

    public abstract Object decode(byte nextByte);

    public abstract void process(int ConnectionID, ConnectionsImpl connections, myBidiMessagingProtocol bidiMessagingProtocol);

    public byte[] shortToBytes(short input){
        byte[] bytes=new byte[2];
        bytes[0]=(byte)(input >> 8 & 0xff);
        bytes[1]=(byte)(input & 0xff);
        return bytes;
    }



}
