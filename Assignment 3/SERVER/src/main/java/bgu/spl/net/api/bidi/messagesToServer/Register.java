package bgu.spl.net.api.bidi.messagesToServer;

import java.nio.charset.StandardCharsets;

import bgu.spl.net.api.bidi.*;
import bgu.spl.net.api.bidi.messagesToClient.Ack;
import bgu.spl.net.api.bidi.messagesToClient.Error;


public class Register extends BasicMessageToServer {
private AllUsers allUsers;
private String name;
private String password;
private byte[] bytes;
private int indexOfUserName;//  //holds the space of the USERNAME in bytes
private int indexOfUserPassword;//////holds the space of the PASSWORD in bytes
private boolean finished;//indicates if we have finished to read USERNAME


    public Register(){
        super();
        bytes=new byte[1<<10];
        indexOfUserName=0;
        indexOfUserPassword=0;
        this.finished=false;
        allUsers = AllUsers.getInstance();
        System.out.println("Register is constructed");
    }



    @Override
    public Object decode(byte nextByte) {
        if(!finished) {//if we aren't finished reading the USERNAME
            if (nextByte != 0) {
                bytes[indexOfUserName] = nextByte;
                indexOfUserName++;
            } else {//we have finished reading the input
                finished = true;
                indexOfUserPassword= indexOfUserName;//we will continue to submit the bytes from this value(from here we save the PASSWORD)
            }
        }
         else{//we have finished reading the USERNAME
             if (nextByte!=0){
                 bytes[indexOfUserPassword]=nextByte;
                 indexOfUserPassword++;
            }
             else{//we have finished to read Both the USERNAME and the PASSWORD
                 name=new String(bytes,0,indexOfUserName, StandardCharsets.UTF_8);
                 password=new String(bytes,indexOfUserName,indexOfUserPassword-1, StandardCharsets.UTF_8);
                 System.out.println("name is "+name+" and password is "+password);
                 return this;
             }
            }
         return null;
    }






    @Override
    public void process(int ConnectionID, ConnectionsImpl connections, myBidiMessagingProtocol bidiMessagingProtocol) {
        //If user is already registered:
        System.out.println("in the register process method");
        if(allUsers.checkIfRegistered(this.name)){
            System.out.println("User is already registered");
            connections.send(ConnectionID,new Error((short)1));//The constructed Error is the response to send back to this client.
        }
        else{//registration of a user
            User user=new User(name,password);
            allUsers.registerUser(name,user);
            connections.send(ConnectionID,new Ack((short)1)); //Succesfull register, Ack is the message sent back to the client.
        }

    }
}
