package bgu.spl.net.api.bidi.messagesToServer;

import bgu.spl.net.api.bidi.*;
import bgu.spl.net.api.bidi.messagesToClient.Ack;
import bgu.spl.net.api.bidi.messagesToClient.Error;
public class Logout extends BasicMessageToServer {

    public Logout(){
        super();
        System.out.println("we are in the logout constructor");
    }
    @Override
    public Object decode(byte nextByte) {
        return this; //there are no strings or chars we need to decode, we are handling the bytes themselves
    }

    @Override
    public void process(int ConnectionID, ConnectionsImpl connections, myBidiMessagingProtocol bidiMessagingProtocol) {
        System.out.println("we are in the LOGOUT process method");
        if (!AllUsers.getInstance().checkIfLoggedIn(ConnectionID))//if user isn't logged in
            connections.send(ConnectionID,new Error((short)3));
        else{
            connections.send(ConnectionID,new Ack((short)3));
            connections.disconnect(ConnectionID);
            AllUsers.getInstance().logOutAUser(ConnectionID);

        }
    }
}
