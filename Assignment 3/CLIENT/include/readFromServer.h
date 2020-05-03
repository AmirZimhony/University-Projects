

#ifndef ASSIGNMENTNUM3CLIENT_READFROMSERVER_H
#define ASSIGNMENTNUM3CLIENT_READFROMSERVER_H

#include "../include/connectionHandler.h"

class readFromServer{
    private: ConnectionHandler& connectionHandler;

    public:
    readFromServer(ConnectionHandler &connectionHandler1);

    void operator()();

    void decode(char *message);

    void PrintNotification();

    void PrintError();

    void PrintAck();

    short bytesToShort(char* bytesArr);

};

































#endif //ASSIGNMENTNUM3CLIENT_READFROMSERVER_H
