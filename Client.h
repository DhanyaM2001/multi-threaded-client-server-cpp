#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include <chrono>
#include <thread>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include "DataTransfer.h"
#include "SocketConnect.h"

class Client : public SocketConnect, public DataTransfer{
private:
    std::string ipAddress;
    int port;

    // Socket operations
   // void createSocket() override;
    void connectOrBind();
    void startCommunication();

public:
    // Constructor
    Client(const std::string& ip, int port);

    // Start method to initialize the connection process
    void start() override;
};

#endif // CLIENT_H

