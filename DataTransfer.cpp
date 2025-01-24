#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include "DataTransfer.h"


void DataTransfer:: sendMessage(int clientSocket) {
        std::string message;
        while (true) {
            std::cout << "Enter message to send (type 'exit' to quit): ";
            std::getline(std::cin, message);

            if (message == "exit") {
                break;
            }

            uint32_t messageLength = htonl(message.size());
            send(clientSocket, reinterpret_cast<char*>(&messageLength), sizeof(messageLength), 0);
            send(clientSocket, message.c_str(), message.size(), 0);

            uint32_t responseLength;
            int bytesReceived = recv(clientSocket, reinterpret_cast<char*>(&responseLength), sizeof(responseLength), 0);
            if (bytesReceived <= 0) {
                std::cerr << "Error receiving response length!" << std::endl;
                break;
            }

            responseLength = ntohl(responseLength);

            std::vector<char> responseBuffer(responseLength);
            bytesReceived = recv(clientSocket, responseBuffer.data(), responseLength, 0);
            if (bytesReceived > 0) {
                std::string response(responseBuffer.begin(), responseBuffer.end());
                std::cout << "Server response: " << response << std::endl;
            } else {
                std::cerr << "Error receiving response!" << std::endl;
                break;
            }
        }
    }

void DataTransfer:: sendFile(int clientSocket) {
        std::string filePath;
        std::cout << "Enter the file path to send: ";
        std::cin.ignore();
        std::getline(std::cin, filePath);

        std::ifstream file(filePath, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filePath << std::endl;
            return;
        }

        std::streamsize fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> fileData(fileSize);
        if (!file.read(fileData.data(), fileSize)) {
            std::cerr << "Failed to read file data!" << std::endl;
            return;
        }

        uint32_t fileSizeNetwork = htonl(static_cast<uint32_t>(fileSize));
        send(clientSocket, reinterpret_cast<char*>(&fileSizeNetwork), sizeof(fileSizeNetwork), 0);
        send(clientSocket, fileData.data(), fileSize, 0);

        std::cout << "File sent successfully!" << std::endl;

        uint32_t fileSizeBackNetwork;
        int bytesReceived = recv(clientSocket, reinterpret_cast<char*>(&fileSizeBackNetwork), sizeof(fileSizeBackNetwork), 0);
        if (bytesReceived <= 0) {
            std::cerr << "Failed to receive the file size back!" << std::endl;
            return;
        }

        uint32_t fileSizeBack = ntohl(fileSizeBackNetwork);
        std::vector<char> fileContentBack(fileSizeBack);
        bytesReceived = recv(clientSocket, fileContentBack.data(), fileSizeBack, 0);
        if (bytesReceived <= 0) {
            std::cerr << "Failed to receive the file content back!" << std::endl;
            return;
        }

//        std::cout << "File content received from server:" << std::endl;
//       std::cout.write(fileContentBack.data(), fileSizeBack);
//      std::cout << std::endl;
//
//      // Prompt the user to choose whether they want to display the file content
	char displayChoice;
	std::cout << "Do you want to display the file content received from the server? (y/n): ";
	std::cin >> displayChoice;
	
	if (displayChoice == 'y' || displayChoice == 'Y') {
    	// Display file content
    		std::cout << "File content received from server:" << std::endl;
    		std::cout.write(fileContentBack.data(), fileSizeBack);
    		std::cout << std::endl;
	} else {
		    // Do not display file content
   		 std::cout << "File content will not be displayed." << std::endl;
	}

  }

void DataTransfer::handleTextMessage(int clientSocket, sockaddr_in clientAddr) {
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
    int clientPort = ntohs(clientAddr.sin_port);

    while (true) {
        uint32_t messageLength;
        int bytesReceived = recv(clientSocket, reinterpret_cast<char*>(&messageLength), sizeof(messageLength), 0);

        if (bytesReceived <= 0) {
            std::cerr << "Client disconnected - " << clientIP << ":" << clientPort << std::endl;
            break;
        }

        messageLength = ntohl(messageLength);
        std::vector<char> buffer(messageLength);
        bytesReceived = recv(clientSocket, buffer.data(), messageLength, 0);

        if (bytesReceived <= 0) {
            std::cerr << "Client disconnected - " << clientIP << ":" << clientPort << std::endl;
            break;
        }

        std::string message(buffer.begin(), buffer.end());
        std::cout << "Received <" << clientIP << "-" << clientPort << "> : " << message << std::endl;

        std::string response = "Server received: " + message;
        uint32_t responseLength = htonl(response.size());
        send(clientSocket, reinterpret_cast<char*>(&responseLength), sizeof(responseLength), 0);
        send(clientSocket, response.c_str(), response.size(), 0);
    }

    close(clientSocket);
}

void DataTransfer::handleFileTransfer(int clientSocket, sockaddr_in clientAddr) {
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
    int clientPort = ntohs(clientAddr.sin_port);

    uint32_t fileSizeNetwork;
    int bytesReceived = recv(clientSocket, reinterpret_cast<char*>(&fileSizeNetwork), sizeof(fileSizeNetwork), 0);
    if (bytesReceived <= 0) {
        std::cerr << "Failed to receive file size!" << std::endl;
        return;
    }

    uint32_t fileSize = ntohl(fileSizeNetwork);
    std::vector<char> fileBuffer(fileSize);
    bytesReceived = recv(clientSocket, fileBuffer.data(), fileSize, 0);
    if (bytesReceived <= 0) {
        std::cerr << "Failed to receive file data!" << std::endl;
        return;
    }

    std::ofstream outFile("received_file.txt", std::ios::binary);
    outFile.write(fileBuffer.data(), fileSize);
    outFile.close();

    std::cout << "File received and saved as 'received_file.txt'" << std::endl;

    std::ifstream file("received_file.txt", std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file for sending back!" << std::endl;
        return;
    }

    file.seekg(0, std::ios::end);
    std::streamsize fileSizeBack = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> fileDataBack(fileSizeBack);
    if (!file.read(fileDataBack.data(), fileSizeBack)) {
        std::cerr << "Failed to read file data!" << std::endl;
        return;
    }

    uint32_t fileSizeBackNetwork = htonl(static_cast<uint32_t>(fileSizeBack));
    send(clientSocket, reinterpret_cast<char*>(&fileSizeBackNetwork), sizeof(fileSizeBackNetwork), 0);
    send(clientSocket, fileDataBack.data(), fileSizeBack, 0);

    std::cout << "File content sent back to the client - " << clientIP << ":" << clientPort << std::endl;
    std::cerr << "Client disconnected - "<<clientIP<<":"<<clientPort << std::endl;
}

