package bgu.spl.net.api.bidi.messagesToClient;

public class Error extends BasicMessageToClient {


    private short type;



    public Error(short type){
        super();
        System.out.println("constructing an error");
        this.type=type;
        setOpCode((short)11);
        encode();
    }

    @Override
    public byte[] encode() {
        byte[] returnValue=new byte[4];
        byte[] additions=shortToBytes(getOpCode());//translation of the OpCode
        returnValue[0]=additions[0];
        returnValue[1]=additions[1];
        additions=shortToBytes(type);//translation of the MessageOpCode
        returnValue[2]=additions[0];
        returnValue[3]=additions[1];

        return returnValue;
    }
}
