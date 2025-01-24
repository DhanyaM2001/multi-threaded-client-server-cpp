# Multi-Threaded Client-Server Application in C++

>Welcome to the Multi-Threaded Client-Server C++ Application! This project demonstrates efficient communication between a client and server using socket programming, thread >pooling, and the Factory Method design pattern in C++. It’s designed to handle both text messages and file transfers, with built-in features to ensure reliable communication.
>Whether you're looking to explore multi-threading in C++, socket communication, or just want to build a solid foundation for your own networking projects, this application >provides a clean and effective approach to client-server interaction.

## Key Features
  - Multi-Threaded Server: Handles multiple clients concurrently with a thread pool to optimize performance.
  - Socket Communication: Establishes reliable client-server communication using TCP sockets.
  - Factory Method Pattern: Dynamically creates client and server socket connections for scalability.
  - Text and File Transfer Support: Send and receive both text messages and files seamlessly.
  - Automated Build: Build and compile the project with a Makefile for easier management.
  - Client Retry Mechanism: Reconnect attempts for failed client-server communication, ensuring resilience.
  - IP & Port Validation: Ensures correct server IP and port before initiating connections.
## How It Works
Components:
- SocketConnect: Handles socket creation and connection with input validation.
- Client: Connects to the server and facilitates data transfer.
- Server: Listens for and manages multiple client connections using threads.
- DataTransfer: Manages sending and receiving text and files.
- SocketFactory: Creates socket connection instances dynamically (Factory Method).
- Main: Entry point to initialize the client or server.
- Makefile: Automates the build and compilation process.
Workflow:
>Client Initiates Connection: The client connects to the server using the IP and port; retries if connection fails.
>Server Listens: Server listens for incoming connections and handles multiple clients using a thread pool.
>Data Transfer: The client sends text or files to the server via DataTransfer.
>Reliable Communication: Ensures proper connection with input validation and retry mechanism.

