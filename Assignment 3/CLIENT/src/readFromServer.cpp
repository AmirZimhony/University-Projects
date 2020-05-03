#include "../include/readFromServer.h"
using namespace std;

short readFromServer::bytesToShort(char *bytesArr){
    short result = (short)((bytesArr[0] & 0xff) << 8);
    result += (short)(bytesArr[1] & 0xff);
    return result;
}


readFromServer::readFromServer(ConnectionHandler &connectionHandler1):connectionHandler(connectionHandler1){}

void readFromServer::operator()() {
    std::cout<<"INSIDE readfromserver run method"<<std::endl;
    while(!connectionHandler.getShouldTerminate()){
        char OpcodeFromServer[2];
        if(connectionHandler.getBytes(OpcodeFromServer,2)){
            short codeFromServ=bytesToShort(OpcodeFromServer);
            if (codeFromServ==9)
                PrintNotification();
            if (codeFromServ==10)
                PrintAck();
            if (codeFromServ==11) {
                PrintError();
                std::cout<<"we recieved an error from the server"<<std::endl;
            }
        }
    }


}


    void readFromServer::PrintNotification() {
        char type[1];
        connectionHandler.getBytes(type,1);
        short t=bytesToShort(type);
        string onScreen="NOTIFICATION ";
        string PostingUser=" ";
        string content=" ";
        connectionHandler.getFrameAscii(PostingUser,'\0');//check this
        connectionHandler.getFrameAscii(content,'\0');
        if (t==0)
            onScreen += "PM"+PostingUser+content;
        else
            onScreen+="Public"+PostingUser+content;

        std::cout<<onScreen<<std::endl;
    }



    void readFromServer::PrintAck() {
        string onScreen="ACK ";
        char msgOpCode[2];
        connectionHandler.getBytes(msgOpCode,2);
        short msg=bytesToShort(msgOpCode);
        string messageOpCode=to_string(msg);
        onScreen+=messageOpCode;
        if (msg==4||msg==7) {//in case the message was a follow/unfollow or userlist
            char NumOfUsers[2];
            connectionHandler.getBytes(NumOfUsers, 2);
            short NumberOfUsers = bytesToShort(NumOfUsers);
            onScreen += to_string(NumberOfUsers);
            for (int i = 0; i < NumberOfUsers; i++) {
                string userName = " ";
                connectionHandler.getFrameAscii(userName, '\0');
                onScreen += userName;
            }
        }
        else if(msg==8){
            char NumOfPosts[2];
            connectionHandler.getBytes(NumOfPosts, 2);
            short NumberOfPosts = bytesToShort(NumOfPosts);
            onScreen += to_string(NumberOfPosts);
            char NumOfFoll[2];
            connectionHandler.getBytes(NumOfFoll, 2);
            short NumberOfFollowers = bytesToShort(NumOfFoll);
            onScreen += to_string(NumberOfFollowers);
            char NumOfFollowing[2];
            connectionHandler.getBytes(NumOfFollowing, 2);
            short NumberOfFollowing = bytesToShort(NumOfFollowing);
            onScreen += to_string(NumberOfFollowing);
         }
        else if(msg==3) {
            connectionHandler.setTermination();
        }

        std::cout<<onScreen<<std::endl;

    }

    void readFromServer::PrintError() {
        string onScreen="Error ";
        char msgOpCode[2];
        connectionHandler.getBytes(msgOpCode,2);
        short msg=bytesToShort(msgOpCode);
        string messageOpCode=to_string(msg);
        onScreen+=messageOpCode;
        std::cout<<onScreen<<std::endl;

    }




