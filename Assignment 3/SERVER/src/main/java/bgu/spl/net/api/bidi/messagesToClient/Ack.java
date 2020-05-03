package bgu.spl.net.api.bidi.messagesToClient;

public class Ack extends BasicMessageToClient {

    private short type;
    private byte[] ACKmessage=new byte[1<<10];
    private byte[] optionalPart = null;
    private byte[] returnValue=new byte[1<<13];


    public Ack(short type){
        super();
        setOpCode((short)10);
        this.type=type;

    }

    public Ack(short type, byte[] optionalPart){
        this(type);
        this.optionalPart = optionalPart;

    }

    @Override
    public byte[] encode() {
        System.out.println("inside encode method of ACK");

        byte[] additions=shortToBytes(getOpCode());//translation of the OpCode
        returnValue[0]=additions[0];
        returnValue[1]=additions[1];
        additions=shortToBytes(type);//translation of the MessageOpCode
        returnValue[2]=additions[0];
        returnValue[3]=additions[1];
        addOptionalPart();
        ACKmessage = returnValue;
        return returnValue;
    }

    private void addOptionalPart() {
        if (optionalPart == null)
            return;
        int ACKindex = 4;
        for (int i = 0; i < optionalPart.length; i++) {
            returnValue[ACKindex] = optionalPart[i];
            ACKindex++;
        }
    }
}

