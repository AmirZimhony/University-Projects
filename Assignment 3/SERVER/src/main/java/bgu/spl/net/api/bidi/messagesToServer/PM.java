package bgu.spl.net.api.bidi.messagesToServer;

import bgu.spl.net.api.bidi.*;
import bgu.spl.net.api.bidi.messagesToClient.Ack;
import bgu.spl.net.api.bidi.messagesToClient.Error;
import bgu.spl.net.api.bidi.messagesToClient.Notification;

import java.nio.charset.StandardCharsets;

public class PM extends BasicMessageToServer {

    private byte[] message;
    private byte[] name;
    private String stringMessage;
    private String stringName;
    private int index;
    private short PmOpCode = 6;
    private AllUsers allUsers;


    public PM() {
        super();
        this.allUsers = AllUsers.getInstance();
        this.index = 0;
        this.message = new byte[1<<10];
        this.name = new byte[1<<10];
        this.stringName = null;
    }

    public Object decode(byte nextByte) {
        if (nextByte == '\0' && this.stringName != null) { // finished reading the whole message.
            this.stringMessage = new String(message, 0, index-1, StandardCharsets.UTF_8);
            return this;
        } else if(nextByte == '\0' && this.stringMessage == null) { // finished reading name
            this.stringName = new String(name, 0 , index-1, StandardCharsets.UTF_8);
            index = 0;
        } else if(stringName == null) { // & nextByte != 0
            name[index] = nextByte;
            index++;
        } else { // stringName != null & nextByte != 0
            message[index] = nextByte;
            index++;
        }
        return null;
    }

    @Override
    public void process(int ConnectionID, ConnectionsImpl connections, myBidiMessagingProtocol bidiMessagingProtocol) {
        /* send an error if:
        1. sending user in not logged in (connectionId)
        2. recipient (user name) is not registered.
        3. message contains '@'
         */
        if (!allUsers.checkIfLoggedIn(ConnectionID) || !allUsers.checkIfRegistered(stringName) || stringMessage.contains("@")) {
            connections.send(ConnectionID, new Error(PmOpCode));
            return;
        }
        Notification notification = new Notification('6', stringName, stringMessage);
        allUsers.getUserByConnectionId(ConnectionID).savePM(notification);
        allUsers.savePM(allUsers.getName(ConnectionID), notification);
        if (allUsers.checkIfLoggedIn(stringName))
            connections.send(allUsers.getConnectionId(stringName),notification);
        else
            allUsers.getUserByName(stringName).addToAwaitingNotifications(notification);
        connections.send(ConnectionID, new Ack(PmOpCode));
    }

}
