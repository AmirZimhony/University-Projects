package bgu.spl.net.api.bidi.messagesToClient;

public abstract class BasicMessageToClient {
private short opCode;

public BasicMessageToClient(){}



public abstract byte[] encode();

public byte[] shortToBytes(short input){
    byte[] bytes=new byte[2];
    bytes[0]=(byte)(input >> 8 & 0xff);
    bytes[1]=(byte)(input & 0xff);
    return bytes;
}

    public void setOpCode(short code){
        opCode=code;
    }

    public short getOpCode() {
        return opCode;
    }
}
