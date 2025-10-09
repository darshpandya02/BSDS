#ifndef SOCKET_COMM_H
#define SOCKET_COMM_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <stdexcept>
#include <cstring>

class SocketComm {
private:
    int sock_fd;
    bool connected;
    
public:
    SocketComm() : sock_fd(-1), connected(false) {}
    
    SocketComm(int fd) : sock_fd(fd), connected(true) {}
    
    ~SocketComm() {
        Close();
    }
    
    // Create and connect socket (client side)
    bool Connect(const std::string& ip, int port) {
        sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (sock_fd < 0) {
            throw std::runtime_error("Failed to create socket");
        }
        
        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        
        if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) {
            close(sock_fd);
            throw std::runtime_error("Invalid IP address");
        }
        
        if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            close(sock_fd);
            throw std::runtime_error("Connection failed");
        }
        
        connected = true;
        return true;
    }
    
    // Send data
    bool Send(const char* data, size_t length) {
        if (!connected || sock_fd < 0) {
            throw std::runtime_error("Socket not connected");
        }
        
        size_t total_sent = 0;
        while (total_sent < length) {
            ssize_t sent = send(sock_fd, data + total_sent, length - total_sent, 0);
            if (sent <= 0) {
                throw std::runtime_error("Send failed");
            }
            total_sent += sent;
        }
        return true;
    }
    
    // Receive data
    bool Receive(char* buffer, size_t length) {
        if (!connected || sock_fd < 0) {
            throw std::runtime_error("Socket not connected");
        }
        
        size_t total_received = 0;
        while (total_received < length) {
            ssize_t received = recv(sock_fd, buffer + total_received, 
                                   length - total_received, 0);
            if (received <= 0) {
                throw std::runtime_error("Receive failed");
            }
            total_received += received;
        }
        return true;
    }
    
    // Close socket
    void Close() {
        if (sock_fd >= 0) {
            close(sock_fd);
            sock_fd = -1;
            connected = false;
        }
    }
    
    int GetFD() const { return sock_fd; }
    bool IsConnected() const { return connected; }
};

class ServerSocket {
private:
    int listen_fd;
    
public:
    ServerSocket() : listen_fd(-1) {}
    
    ~ServerSocket() {
        Close();
    }
    
    // Create and bind listening socket
    bool Listen(int port, int backlog = 5) {
        listen_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (listen_fd < 0) {
            throw std::runtime_error("Failed to create listening socket");
        }
        
        // Allow reuse of address
        int opt = 1;
        setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        
        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(port);
        
        if (bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            close(listen_fd);
            throw std::runtime_error("Bind failed");
        }
        
        if (listen(listen_fd, backlog) < 0) {
            close(listen_fd);
            throw std::runtime_error("Listen failed");
        }
        
        return true;
    }
    
    // Accept incoming connection
    SocketComm* Accept() {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        
        int client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            throw std::runtime_error("Accept failed");
        }
        
        return new SocketComm(client_fd);
    }
    
    void Close() {
        if (listen_fd >= 0) {
            close(listen_fd);
            listen_fd = -1;
        }
    }
};

#endif // SOCKET_COMM_H