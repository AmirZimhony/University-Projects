package bgu.spl.net.api.bidi.messagesToServer;

import bgu.spl.net.api.bidi.*;
import bgu.spl.net.api.bidi.messagesToClient.Ack;
import bgu.spl.net.api.bidi.messagesToClient.Error;
import java.nio.charset.StandardCharsets;

public class Stat extends BasicMessageToServer {

    private int index;
    private byte[] nameInBytes;
    private String name;
    private byte[] toAdd;
    private AllUsers allUsers;
    private short StatOpCode;
    private User user;
    private byte[] ACKmessage;

    public Stat() {
        super();
        this.index = 0;
        this.nameInBytes = new byte[1<<10];
        this.toAdd = new byte[2];
        this.allUsers = AllUsers.getInstance();
        this.StatOpCode = 8;
        this.ACKmessage = new byte[1<<10];
    }

    public Object decode(byte nextByte) {
        if (nextByte == '\0') {
            name = new String(nameInBytes, 0, index, StandardCharsets.UTF_8);
            return this;
        } else {
            nameInBytes[index] = nextByte;
            index++;
        }
        return null;
    }

    public void process(int ConnectionID, ConnectionsImpl connections, myBidiMessagingProtocol bidiMessagingProtocol) {
        if (!allUsers.checkIfLoggedIn(ConnectionID)) {
            connections.send(ConnectionID, new Error(StatOpCode));
            return;
        }

        this.user = allUsers.getUserByConnectionId(ConnectionID);
        index = 0;
        // preparing ACK message:
        toAdd = shortToBytes(StatOpCode);
        addToACKmessage();
        toAdd = shortToBytes((short)user.getNumOfPosts());
        addToACKmessage();
        toAdd = shortToBytes((short)user.getNumOfFollowers());
        addToACKmessage();
        toAdd = shortToBytes((short)user.getNumOfFollowing());
        addToACKmessage();

        connections.send(ConnectionID, new Ack(StatOpCode, ACKmessage));
    }

    private void addToACKmessage() {
        for (int i = 0; i < 2; i++) {
            ACKmessage[index] = toAdd[i];
            index++;
        }
    }
    public byte[] shortToBytes(short num) {
        byte[] bytesArr = new byte[2];
        bytesArr[0] = (byte)((num >> 8) & 0xFF);
        bytesArr[1] = (byte)(num & 0xFF);
        return bytesArr;
    }
}
