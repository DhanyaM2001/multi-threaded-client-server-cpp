#ifndef SOCKETCONNECT_H
#define SOCKETCONNECT_H

#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

class SocketConnect {
protected:
    int socketFd;   
    std::string ipAddress;
    int port;

    // Common interface
    void createSocket();
    //virtual void connectOrBind() = 0;
    //virtual void startCommunication() = 0;

public:
    virtual ~SocketConnect();

    virtual void start() = 0;

    bool validateIpAddress(const std::string& ipAddress);
    bool validatePort(const std::string& portStr);
    std::string getInputIpAddress();
    int getInputPort();
};

#endif // SOCKETCONNECT_H

