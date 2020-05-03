#ifndef READFROMKEYBOARD__
#define READFROMKEYBOARD__

#include "connectionHandler.h"
#include <mutex>


using namespace std;


class readFromKeyboard{

private:
    ConnectionHandler &connectionHandler;
    bool Terminate;


public:
    readFromKeyboard(ConnectionHandler &connectionHandler1);


    void operator()();

    bool encode(std::string input);// my way, look at parsing in "start" method in restaurant (assignment 1)

    bool Register(vector<string> parameters);

    bool Login(vector<string> parameters);

    bool Logout();

    bool Follow (vector<string> parameters);

    bool Post (vector<string> parameters);

    bool PrivateMessage (vector<string> parameters);

    bool UserList();

    bool Stat (vector<string> parameters);

};

#endif