#include <stdlib.h>
#include "../include/connectionHandler.h"
#include "../include/readFromKeyboard.h"
#include "../include/readFromServer.h"
#include <thread>
using namespace std;
/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main (int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }
    std::string host = argv[1];//host name
    short port = atoi(argv[2]);//host port number

    ConnectionHandler connectionHandler(host, port);
    if (!connectionHandler.connect()) {//if connection was not established
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }

    readFromKeyboard readFromKeyboard1(connectionHandler);
    readFromServer readFromServer1(connectionHandler);

    thread thread1(ref(readFromServer1));
    thread thread2(ref(readFromKeyboard1));

    thread1.join();
    thread2.join();

    return 0;
}
