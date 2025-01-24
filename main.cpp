#include <iostream>
#include "SocketFactory.cpp"

int main() {
    int choice;
    std::cout << "Choose an option:\n";
    std::cout << "1. Start as Client\n";
    std::cout << "2. Start as Server\n";
    std::cout << "Enter your choice (1 or 2): ";
    std::cin >> choice;

    SocketConnect* connection = nullptr;

    try {
        if (choice == 1) {
	    std::string ip = connection->getInputIpAddress();
            int port = connection->getInputPort();

            connection = SocketFactory::createSocketConnection(choice, ip, port);
        } else if (choice == 2) {
	    int port = connection->getInputPort();
            connection = SocketFactory::createSocketConnection(choice, "", port);
        }
	else {
	   std::cout << "Invalid Choice...";
	}


	if (connection) {  // Check if connection is valid
            connection->start();  // This should now work if start is implemented
        }

        delete connection;
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return -1;
    }

    return 0;
}

