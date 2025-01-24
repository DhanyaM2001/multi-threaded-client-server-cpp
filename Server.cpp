#include "Server.h"

Server::Server(int port) : port(port), serverRunning(true) {
    socketFd = 0; // Initialize socketFd if needed
}


void Server::start(){
    createSocket();
    connectOrBind();

    const int initialThreadPoolSize = std::thread::hardware_concurrency();
    for (int i = 0; i < initialThreadPoolSize; ++i) {
        threadPool.emplace_back(&Server::workerThread, this);
    }

    startCommunication();

    serverRunning = false;
    conditionVar.notify_all();

    for (auto& th : threadPool) {
        if (th.joinable()) {
            th.join();
        }
    }

    close(socketFd);
}

void Server::stop() {
    serverRunning = false;
    conditionVar.notify_all();
    close(socketFd);
}


void Server::connectOrBind() {
   sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    hint.sin_addr.s_addr = INADDR_ANY;

    if (bind(socketFd, (struct sockaddr*)&hint, sizeof(hint)) < 0) {
        std::cerr << "Can't bind to IP/port! Quitting" << std::endl;
        close(socketFd);
        exit(-1);
    }

    if (listen(socketFd, SOMAXCONN) < 0) {
        std::cerr << "Can't listen! Quitting" << std::endl;
        close(socketFd);
        exit(-1);
    }

    std::cout << "Server is listening on port " << port << "..." << std::endl;
}



void Server::startCommunication() {
    while (serverRunning) {
        sockaddr_in clientAddr;
        socklen_t clientSize = sizeof(clientAddr);

        int clientSocket = accept(socketFd, (struct sockaddr*)&clientAddr, &clientSize);
        if (clientSocket < 0) {
            std::cerr << "Accept failed!" << std::endl;
            continue;
        }

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            clientQueue.push(std::make_pair(clientSocket, clientAddr));
        }

        conditionVar.notify_one();
    }
}

void Server::workerThread() {
    while (serverRunning) {
        std::unique_lock<std::mutex> lock(queueMutex);
        conditionVar.wait(lock, [this] { return !clientQueue.empty() || !serverRunning; });

        if (!serverRunning) {
            break;
        }

        auto clientPair = clientQueue.front();
        clientQueue.pop();
        lock.unlock();

        int clientSocket = clientPair.first;
        sockaddr_in clientAddr = clientPair.second;

        handleClient(clientSocket, clientAddr);
    }
}

void Server::handleClient(int clientSocket, sockaddr_in clientAddr) {
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
    int clientPort = ntohs(clientAddr.sin_port);

    std::cout << "Thread " << std::this_thread::get_id() << " is handling client: " << clientIP << ":" << clientPort << std::endl;

    int choice;
    recv(clientSocket, reinterpret_cast<char*>(&choice), sizeof(choice), 0);

    if (choice == 1) {
        std::cout << "Client chose to send a message." << std::endl;
        handleTextMessage(clientSocket, clientAddr);
    } else if (choice == 2) {
        std::cout << "Client chose to send a file." << std::endl;
        handleFileTransfer(clientSocket, clientAddr);
    }

    close(clientSocket);
}



