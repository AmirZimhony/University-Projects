package bgu.spl.net.api.bidi.messagesToClient;



public class Notification extends BasicMessageToClient {
private char TypeOfMessage;//can have 2 values-0(PM) or 1(public message)
private String PostingUser;//name of user who posted/sent the message
private String Content;//the message itself(that was posted/sent)
private int indexOfUserName;//change name later  //holds the space of the USERNAME in bytes

    public Notification(char typeOfMessage,String postingUser, String content){
        super();
        this.TypeOfMessage=typeOfMessage;
        this.PostingUser=postingUser;
        this.Content=content;
        setOpCode((short)9);
        indexOfUserName=0;

    }


    @Override
    public byte[] encode() {
        byte[] returnValue=new byte[1<<10];
        byte[] additions=shortToBytes(getOpCode());//translation of the OpCode
        returnValue[0]=additions[0];
        returnValue[1]=additions[1];
        byte b = (byte)TypeOfMessage;
        returnValue[2]=b;//the message type(PM/PUBLIC) converted to a byte
        additions=PostingUser.getBytes();//convert Posting userName to bytes
        for(int i=0;i<additions.length;i++){//putting the Posting userName bytes in the bytes array
            returnValue[3+i]=additions[i];
        }
        returnValue[3+additions.length]=0;//so we know the UserName has ended
        indexOfUserName=3+additions.length+1;//from this index it is safe to write in the bytes array
        additions=Content.getBytes();
        for(int j=0;j<additions.length;j++){//putting the content bytes in the bytes array
            returnValue[indexOfUserName+j]=additions[j];
        }
        returnValue[indexOfUserName+additions.length]=0;//so we know the content has ended

        return returnValue;

    }
}
