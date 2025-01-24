#include "Client.h"

Client::Client(const std::string& ip, int port) {
    ipAddress = ip;
    this->port = port;
}


void Client::connectOrBind(){
        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr);

        int retryCount = 0;
        bool success = false;

        while (retryCount < 3 && !success) {
            int attempt = 0;
            while (attempt < 3) {
                if (connect(socketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
                    attempt++;
                    std::cerr << "Connection attempt " << attempt << " failed! Retrying..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                } else {
                    success = true;
                    break;
                }
            }

            if (success) {
                std::cout << "Connected to server." << std::endl;
                break;
            }

            retryCount++;
            if (retryCount < 3) {
                std::cout << "Do you want to (1) retry with the same IP and port or (2) use new IP and port? Enter 1 or 2: ";
                int option;
                std::cin >> option;

                if (option == 2) {
                    getInputIpAddress();
                    getInputPort();
                } else if (option != 1) {
                    std::cerr << "Invalid option. Exiting." << std::endl;
                    close(socketFd);
                    exit(EXIT_FAILURE);
                }
            } else {
                std::cerr << "Maximum retry attempts reached. Exiting." << std::endl;
                close(socketFd);
                exit(EXIT_FAILURE);
            }
        }

    }


void Client::startCommunication(){
        std::cin.ignore();
        int choice;
        std::cout << "Select an option:\n1. Send Text Message\n2. Send File\nEnter choice: ";
        std::cin >> choice;

        send(socketFd, reinterpret_cast<char*>(&choice), sizeof(choice), 0);

        if (choice == 1) {
            std::cin.ignore();
            sendMessage(socketFd);
        } else if (choice == 2) {
            sendFile(socketFd);
        } else {
            std::cerr << "Invalid choice! Please select either 1 or 2." << std::endl;
        }
    }


void Client::start() {
	createSocket();   
        connectOrBind();
        startCommunication();
        close(socketFd);
    }

