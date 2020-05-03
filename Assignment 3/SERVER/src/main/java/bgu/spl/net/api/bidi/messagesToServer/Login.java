package bgu.spl.net.api.bidi.messagesToServer;

import bgu.spl.net.api.bidi.*;
import bgu.spl.net.api.bidi.messagesToClient.Error;
import java.nio.charset.StandardCharsets;
import bgu.spl.net.api.bidi.messagesToClient.Ack;
import bgu.spl.net.api.bidi.messagesToClient.Notification;


public class Login extends BasicMessageToServer {
    private String name;
    private String password;
    private byte[] bytes;
    private int indexOfUserName;//change name later  //holds the space of the USERNAME in bytes
    private int indexOfUserPassword;//change name later  ////holds the space of the PASSWORD in bytes
    private boolean finished;//indicates if we have finished to read USERNAME




    public Login(){
        super();
        bytes=new byte[1<<10];
        indexOfUserName=0;
        indexOfUserPassword=0;
        this.finished=false;
        System.out.println("login is constructed");
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
                return this;
            }
        }
        return null;
    }


    @Override
    public void process(int ConnectionID, ConnectionsImpl connections, myBidiMessagingProtocol bidiMessagingProtocol) {
        System.out.println("IN LOGIN PROCESS METHOD");
        AllUsers allUsers=AllUsers.getInstance();
        if(!allUsers.checkIfRegistered(name)) {//if user does not exist AKA has not registered
            System.out.println("User is not registered");
            connections.send(ConnectionID,new Error((short)2));//The constructed Error is the response to send back to this client.
        }
        else if (allUsers.checkIfLoggedIn(ConnectionID)) {//if the client is already logged in
            System.out.println("User is already logged in");
            connections.send(ConnectionID,new Error((short)2));//The constructed Error is the response to send back to this client.
        }
        else if (!allUsers.getUserByName(name).getPassword().equals(password)) {//if current password doesn't match the one the user registered with
            System.out.println("Wrong password");
            connections.send(ConnectionID,new Error((short)2));//The constructed Error is the response to send back to this client.
        }
        else {
            User user=allUsers.getUserByName(name);
            user.setConnectionID(ConnectionID);
            allUsers.logInAUser(name,user);
            allUsers.MapConnection(ConnectionID,name);
            //before client recieves Ack message, he first recieves all of the notifications he was sent while he was offline.
            connections.send(ConnectionID,new Ack((short)2)); //Succesfull log in, Ack is the message sent back to the client
            if(!user.getAwaitingNotifications().isEmpty()) {
                for (Notification notification : user.getAwaitingNotifications())
                    connections.send(ConnectionID, notification);
            }

        }


    }
}

