#include "SocketConnect.h"
#include "Client.h"
#include "Server.h"

class SocketFactory {
public:
    static SocketConnect* createSocketConnection(int choice, const std::string& ip, int port) {
        if (choice == 1) {
            return new Client(ip, port);  // Client
        } else if (choice == 2) {
            return new Server(port);  // Server
        } else {
            throw std::invalid_argument("Invalid choice");
        }
    }
};


