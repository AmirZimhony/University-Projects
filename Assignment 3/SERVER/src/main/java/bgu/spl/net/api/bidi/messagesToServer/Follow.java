package bgu.spl.net.api.bidi.messagesToServer;

import bgu.spl.net.api.bidi.*;
import bgu.spl.net.api.bidi.messagesToClient.Ack;
import bgu.spl.net.api.bidi.messagesToClient.Error;

import java.nio.charset.StandardCharsets;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

public class Follow extends BasicMessageToServer {

    private boolean sendErrorMessage;// changes to false once one of the un/follow succeeds.
    private int index;
    private int userIndex;
    private int followOrUnfollow;
    private byte[] bytesUserName;
    private byte[] bytesNumOfUsers;
    private short numOfUsers;
    private String userName;
    private List<String> userNameList;
    private AllUsers allUsers;
    private short numberOfSuccessfulFollowsOrUnfollows;
    private List<String> namesOfSuccessfullFollowsOrUnfollows;
    private short FollowOpCode;

    public Follow() {
        super();
        allUsers = AllUsers.getInstance();
        this.sendErrorMessage = true;
        this.index = 0;
        this.followOrUnfollow = -1;
        this.bytesNumOfUsers = new byte[2];
        this.bytesUserName = new byte[1<<10];
        this.userIndex = 0;
        this.userNameList = new CopyOnWriteArrayList<>();
        this.numberOfSuccessfulFollowsOrUnfollows = 0;
        this.namesOfSuccessfullFollowsOrUnfollows = new CopyOnWriteArrayList<>();
        this.FollowOpCode = 4;

    }


    public Object decode(byte nextByte) {
        if (index == 0) { // this byte is follow/unfollow
            followOrUnfollow = nextByte;
            index++;
        } else if (index == 1) { // the following two bytes represent the number of users to follow/unfollow
            bytesNumOfUsers[0] = nextByte;
            index++;
        } else if (index == 2) {
            bytesNumOfUsers[1] = nextByte;
            index++;
            numOfUsers = bytesToShort(bytesNumOfUsers);
        } else {
            if (nextByte != '\0' && index > 2) {
                bytesUserName[userIndex] = nextByte;
                index++;
                userIndex++;
            } else {
                userName = new String(bytesUserName, 0, userIndex-1, StandardCharsets.UTF_8);
                userNameList.add(userName);
                index++;
                userIndex = 0;
                if (userNameList.size() == numOfUsers)
                    return this;
            }
        }
        return null;
    }

    public void process(int ConnectionID, ConnectionsImpl connections, myBidiMessagingProtocol bidiMessagingProtocol) {
        // user isn't logged in/ userList id empty - send an error message:
        if (!allUsers.checkIfLoggedIn(ConnectionID) || numOfUsers == 0) {
            connections.send(ConnectionID, new Error(FollowOpCode));
            return;
        }
        // user is logged in: send an error iff all follow/unfollow are unsuccessful.
        User user = allUsers.getUserByConnectionId(ConnectionID);
        userName = user.getName();
        boolean success = false; // the success of each un/follow
            for (String tmp : userNameList) {
                success = user.followOrUnfollow(followOrUnfollow, tmp);
                if (success) {
                    numberOfSuccessfulFollowsOrUnfollows++;
                    namesOfSuccessfullFollowsOrUnfollows.add(tmp);
                    allUsers.getUserByName(tmp).followerOrUnfollower(followOrUnfollow,userName);
                }
                if (sendErrorMessage && success)
                    sendErrorMessage = false;
            }
            if (sendErrorMessage) {
                connections.send(ConnectionID, new Error((short) 4));
                return;
            }

            encodeACKmessage(ConnectionID, connections);
    }

    private void encodeACKmessage(int ConnectionID, Connections connections) {

        // prepare ACK:
        byte[] ACKmessage = new byte[1<<10];
        // add number of successful un/follows to ACK
        byte[] numberOfSuccessfulFollowsOrUnfollowsBytes = shortToBytes(numberOfSuccessfulFollowsOrUnfollows);
        ACKmessage[0] = numberOfSuccessfulFollowsOrUnfollowsBytes[0];
        ACKmessage[1] = numberOfSuccessfulFollowsOrUnfollowsBytes[1];
        int index = 2;
        // add all relevant user names
        for (String name: namesOfSuccessfullFollowsOrUnfollows) {
            byte[] nameInBytes = name.getBytes();
            for (int i = 0; i < nameInBytes.length; i++) {
                ACKmessage[index] = nameInBytes[i];
                index++;
            }
            ACKmessage[index] = 0;
            index++;
        }

        // send to ACK message to complete
        connections.send(ConnectionID, new Ack(FollowOpCode, ACKmessage));

    }
    private short bytesToShort (byte[] byteArr) {
        short result = (short) ((byteArr[0] & 0xff) << 8);
        result += (short) (byteArr[1] & 0xff);
        return result;
    }

}
