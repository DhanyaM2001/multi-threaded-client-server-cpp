#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <fstream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "DataTransfer.h"
#include "SocketConnect.h"

class Server : public SocketConnect, public DataTransfer {
public:
    Server(int port);
    void start() override;
    void stop();
private:
   // void createSocket() override;
    void connectOrBind();
    void startCommunication();
    void handleClient(int clientSocket, sockaddr_in clientAddr);
    void workerThread();
   
    int port;
    bool serverRunning;

    std::mutex queueMutex;
    std::condition_variable conditionVar;
    std::queue<std::pair<int, sockaddr_in>> clientQueue;
    std::vector<std::thread> threadPool;
};

#endif
