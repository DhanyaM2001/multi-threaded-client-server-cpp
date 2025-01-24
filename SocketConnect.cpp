#include "SocketConnect.h"

SocketConnect::~SocketConnect() {
    close(socketFd);
}

void SocketConnect::createSocket() {
    socketFd = socket(AF_INET, SOCK_STREAM, 0);  // Initialize the socketFd from the base class
    if (socketFd == -1) {
        std::cerr << "Socket creation failed!" << std::endl;
        exit(EXIT_FAILURE);
    }
  }


bool SocketConnect::validateIpAddress(const std::string& ipAddress) {
    struct sockaddr_in sa;
    return inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr)) == 1;
}

bool SocketConnect::validatePort(const std::string& portStr) {
    for (char c : portStr) {
        if (!isdigit(c)) return false;
    }
    int port = std::stoi(portStr);
    return port > 0 && port <= 65535;
}

std::string SocketConnect::getInputIpAddress() {
    std::string ipAddress;     
    while (true) {
        std::cout << "Enter server IP address: ";
        std::cin >> ipAddress;

        if (validateIpAddress(ipAddress)) {
            return ipAddress;
        } else {
            std::cerr << "Invalid IP address! Please enter again." << std::endl;
        }
    }
}

int SocketConnect::getInputPort() {
    std::string portStr;
    while (true) {
        std::cout << "Enter server port: ";
        std::cin >> portStr;

        if (validatePort(portStr)) {
            return std::stoi(portStr);
        } else {
            std::cerr << "Invalid port! Please enter a valid port between 1 and 65535." << std::endl;
        }
    }
}



