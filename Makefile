# Compiler and Flags
CXX = g++
CXXFLAGS = -Wall -fPIC -lpthread -I.

# Directories and Libraries
TARGET = main
LIBS = -L. -lClient -lServer

# Shared Libraries
LIBCLIENT = libClient.so
LIBSERVER = libServer.so

# Object Files
OBJ_CLIENT = Client.o
OBJ_SERVER = Server.o
OBJ_DATA_TRANSFER = DataTransfer.o
OBJ_SOCKET_CONNECT = SocketConnect.o
OBJ_SOCKET_FACTORY = SocketFactory.o

# Build Targets
all: $(TARGET) run

# Compile main program and link with shared libraries
$(TARGET): main.o $(LIBCLIENT) $(LIBSERVER)
	$(CXX) -o $(TARGET) main.o $(LIBS) $(CXXFLAGS) -Wl,-rpath=.

# Compile main.o
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

# Build the shared library for Client
$(LIBCLIENT): $(OBJ_CLIENT) $(OBJ_DATA_TRANSFER) $(OBJ_SOCKET_CONNECT)
	$(CXX) -shared -o $(LIBCLIENT) $(OBJ_CLIENT) $(OBJ_DATA_TRANSFER) $(OBJ_SOCKET_CONNECT) $(CXXFLAGS)

# Build the shared library for Server
$(LIBSERVER): $(OBJ_SERVER) $(OBJ_DATA_TRANSFER) $(OBJ_SOCKET_CONNECT)
	$(CXX) -shared -o $(LIBSERVER) $(OBJ_SERVER) $(OBJ_DATA_TRANSFER) $(OBJ_SOCKET_CONNECT) $(CXXFLAGS)

# Compile Client.o
Client.o: Client.cpp Client.h SocketConnect.h DataTransfer.h
	$(CXX) $(CXXFLAGS) -c Client.cpp

# Compile Server.o
Server.o: Server.cpp Server.h SocketConnect.h DataTransfer.h
	$(CXX) $(CXXFLAGS) -c Server.cpp

# Compile DataTransfer.o
DataTransfer.o: DataTransfer.cpp DataTransfer.h
	$(CXX) $(CXXFLAGS) -c DataTransfer.cpp

# Compile SocketConnect.o
SocketConnect.o: SocketConnect.cpp SocketConnect.h
	$(CXX) $(CXXFLAGS) -c SocketConnect.cpp

# Compile SocketFactory.o
SocketFactory.o: SocketFactory.cpp SocketFactory.h
	$(CXX) $(CXXFLAGS) -c SocketFactory.cpp

# Run the executable after building
run: $(TARGET)
	./$(TARGET)

# Clean build files
clean:
	rm -f *.o $(LIBCLIENT) $(LIBSERVER) $(TARGET)

