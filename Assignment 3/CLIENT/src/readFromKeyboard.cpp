

#include "../include/readFromKeyboard.h"
#include <vector>
#include <iostream>
#include "../include/connectionHandler.h"


void shortToBytes(short num, char* bytesArr){
    bytesArr[0] = ((num >> 8) & 0xFF);
    bytesArr[1] = (num & 0xFF);
}




readFromKeyboard::readFromKeyboard(ConnectionHandler &connectionHandler1): connectionHandler(connectionHandler1){}

void  readFromKeyboard::operator()() {

    while (!connectionHandler.getShouldTerminate()){
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        std::string line(buf);
        {
            if (!encode(line)) {
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }
        }
        if(line== "LOGOUT"&&connectionHandler.getConnectedStatus()){
            break;

        }
    }
    connectionHandler.setTermination();
}

bool readFromKeyboard::encode(std::string input) {
    vector<string> partsOfSent;
    stringstream check2(input);
    string part;
    while (getline(check2, part, ' ')) {//NEED TO CHECK THIS WORKS
        partsOfSent.push_back(part);
    }
    std::cout << partsOfSent.at(0)<< std::endl;// check first word of input

    if (partsOfSent.at(0)=="REGISTER")
        return Register(partsOfSent);
    else if(partsOfSent.at(0)=="LOGIN")
        return Login(partsOfSent);
    else if (partsOfSent.at(0)=="LOGOUT")
        return Logout();
    else if (partsOfSent.at(0)=="FOLLOW")
        return Follow(partsOfSent);
    else if (partsOfSent.at(0)=="POST")
        return Post(partsOfSent);
    else if (partsOfSent.at(0)=="PM")
        return PrivateMessage(partsOfSent);
    else if (partsOfSent.at(0)=="USERLIST")
        return UserList();
    else if (partsOfSent.at(0)=="STAT")
        return Stat(partsOfSent);


}
//********defining functions for every possible message client can send to server

bool readFromKeyboard::Register(vector<string> parameters) {
    int LengthOfBytes=parameters.at(1).size()+parameters.at(2).size()+4;//enough room for username, password, and opcode(2 bytes) and 2 spaces(2 bytes)
    char bytesToSend[LengthOfBytes];
    shortToBytes((short)1,bytesToSend);//now bytesToSend holds the opCode at index 0 and 1
    int pointerInBytes=2;
    string UserName=parameters.at(1);
    vector<char> userNameVec(UserName.c_str(),UserName.c_str()+UserName.size());//NEED TO CHECK THIS WORKS
    for (auto character: userNameVec){
        bytesToSend[pointerInBytes]=character;
        pointerInBytes++;
    }
    bytesToSend[pointerInBytes]='\0';
    pointerInBytes++;
    string PassWord=parameters.at(2);
    vector<char> PassWordVec(PassWord.c_str(),PassWord.c_str()+PassWord.size());//NEED TO CHECK THIS WORKS
    for (auto character: PassWordVec){
        bytesToSend[pointerInBytes]=character;
        pointerInBytes++;
    }
    bytesToSend[pointerInBytes]='\0';

    return connectionHandler.sendBytes(bytesToSend,LengthOfBytes);
}


bool readFromKeyboard::Login(vector<string> parameters) {
    connectionHandler.ConnectOnline();
    int LengthOfBytes=parameters.at(1).size()+parameters.at(2).size()+4;//enough room for username, password, and opcode(2 bytes) and 2 spaces(2 bytes)
    char bytesToSend[LengthOfBytes];
    shortToBytes((short)2,bytesToSend);//now bytesToSend holds the opCode at index 0 and 1
    int pointerInBytes=2;
    string UserName=parameters.at(1);
    vector<char> userNameVec(UserName.c_str(),UserName.c_str()+UserName.size());//NEED TO CHECK THIS WORKS
    for (auto character: userNameVec){
        bytesToSend[pointerInBytes]=character;
        pointerInBytes++;
    }
    bytesToSend[pointerInBytes]='\0';
    pointerInBytes++;
    string PassWord=parameters.at(2);
    vector<char> PassWordVec(PassWord.c_str(),PassWord.c_str()+PassWord.size());//NEED TO CHECK THIS WORKS
    for (auto character: PassWordVec){
        bytesToSend[pointerInBytes]=character;
        pointerInBytes++;
    }
    bytesToSend[pointerInBytes]='\0';
    return connectionHandler.sendBytes(bytesToSend,LengthOfBytes);
}


bool readFromKeyboard::Logout() {
    char bytesToSend[2];
    shortToBytes((short)3,bytesToSend);
    return connectionHandler.sendBytes(bytesToSend,2);
}

bool readFromKeyboard::Follow(vector<string> parameters) {
    int usersWeFollowSize=0;
    for(int i=3;i<parameters.size();i++) {//usersWeFollowSize will hold the length of all the users we want to follow
        usersWeFollowSize += parameters.at(i).size();//now we know how big the message we are going to send needs to be
    }
    short FollowOrUnfollow=static_cast<short>(std::stoi(parameters.at(1)));
    short NumToFollow= static_cast<short>(std::stoi(parameters.at(2)));
    int LengthOfBytes=5+usersWeFollowSize+NumToFollow;//size of userNames and number of '0' between them,plus 5 bytes for everything else
    char bytesToSend[LengthOfBytes];
    shortToBytes((short)4,bytesToSend);
    bytesToSend[2]=char(FollowOrUnfollow);
    char bytesOFNumToFollow[2];
    shortToBytes(NumToFollow,bytesOFNumToFollow);
    bytesToSend[3]=bytesOFNumToFollow[0];
    bytesToSend[4]=bytesOFNumToFollow[1];
    int  pointerInBytes=5;
    string currentUserName;
    for(int i=3;i<parameters.size();i++) {//iterate through all user names
        currentUserName = parameters.at(i);
        vector<char> currentUserNameVec(currentUserName.c_str(),currentUserName.c_str()+currentUserName.size());
        for (auto character: currentUserNameVec) {
            bytesToSend[pointerInBytes] = character;
            pointerInBytes++;
        }
        bytesToSend[pointerInBytes]='0';
    }

    return connectionHandler.sendBytes(bytesToSend,LengthOfBytes);


}

bool readFromKeyboard::Post(vector<string> parameters) {
    int LengthOfBytes=3+parameters.at(1).size();
    char bytesToSend[LengthOfBytes];
    string postMsg=parameters.at(1);
    vector<char> postMsgVec(postMsg.c_str(),postMsg.c_str()+postMsg.size());
    shortToBytes((short)5,bytesToSend);
    int pointerInBytes=2;
    for (auto character: postMsgVec) {
        bytesToSend[pointerInBytes] = character;
        pointerInBytes++;
    }
    bytesToSend[pointerInBytes]='0';
    return connectionHandler.sendBytes(bytesToSend,LengthOfBytes);
}

bool readFromKeyboard::PrivateMessage(vector<string> parameters) {
    int LengthOfBytes=4+parameters.at(1).size()+parameters.at(2).size();
    string UserName=parameters.at(1);
    string content=parameters.at(2);
    vector<char> UserNameVec(UserName.c_str(),UserName.c_str()+UserName.size());
    vector<char> contentVec(content.c_str(),content.c_str()+content.size());
    char bytesToSend[LengthOfBytes];
    shortToBytes((short)6,bytesToSend);
    int pointerInBytes=2;
    for(auto character: UserNameVec){
        bytesToSend[pointerInBytes]=character;
        pointerInBytes++;
    }
    bytesToSend[pointerInBytes]='\0';
    pointerInBytes++;
    for(auto character: contentVec){
        bytesToSend[pointerInBytes]=character;
        pointerInBytes++;
    }
    bytesToSend[pointerInBytes]='\0';

    return  connectionHandler.sendBytes(bytesToSend,LengthOfBytes);
}



bool readFromKeyboard::UserList() {
    char bytesToSend[2];
    shortToBytes((short)7,bytesToSend);
    return connectionHandler.sendBytes(bytesToSend,2);
}

bool readFromKeyboard::Stat(vector<string> parameters) {
    int LengthOfBytes=3+parameters.at(1).size();
    char bytesToSend[LengthOfBytes];
    string UserName=parameters.at(1);
    vector<char> UserNameVec(UserName.c_str(),UserName.c_str()+UserName.size());
    shortToBytes((short)8,bytesToSend);
    int pointerInBytes=2;
    for(auto character: UserNameVec){
        bytesToSend[pointerInBytes]=character;
        pointerInBytes++;
    }
    bytesToSend[pointerInBytes]='\0';

    return connectionHandler.sendBytes(bytesToSend,2);
}













