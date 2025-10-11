CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pthread -O2
INCLUDES = -I.

# Source files
CLIENT_SRC = ClientMain.cpp
SERVER_SRC = ServerMain.cpp

# Header dependencies
HEADERS = Common.h SocketComm.h ClientStub.h ServerStub.h ExpertQueue.h

# Targets
all: client server

client: $(CLIENT_SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o client $(CLIENT_SRC)

server: $(SERVER_SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o server $(SERVER_SRC)

# Clean build artifacts
clean:
	rm -f client server

# Quick test with 4 customers, 100 orders each
test: client server
	@echo "Starting test server..."
	./server 12345 2 &
	@sleep 2
	@echo "Running test client..."
	./client 127.0.0.1 12345 4 100 0
	@echo "Stopping server..."
	@pkill -f "./server" || true
	@sleep 1

.PHONY: all clean test