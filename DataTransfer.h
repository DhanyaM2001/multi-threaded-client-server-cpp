#ifndef DATA_TRANSFER_H
#define DATA_TRANSFER_H

#include <netinet/in.h> // For sockaddr_in

class DataTransfer {
protected:
    void sendMessage(int clientSocket);
    void sendFile(int clientSocket);
    void handleTextMessage(int clientSocket, sockaddr_in clientAddr);
    void handleFileTransfer(int clientSocket, sockaddr_in clientAddr);
};

#endif
