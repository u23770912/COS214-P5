#include "NetworkServer.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <iostream>

NetworkServer::NetworkServer() 
    : serverSocket(-1), clientSocket(-1), running(false) {
}

NetworkServer::~NetworkServer() {
    stop();
}

bool NetworkServer::start(int port) {
    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return false;
    }
    
    // Allow socket reuse
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // Bind to port
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
    
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Failed to bind to port " << port << std::endl;
        close(serverSocket);
        return false;
    }
    
    // Listen for connections
    if (listen(serverSocket, 1) < 0) {
        std::cerr << "Failed to listen" << std::endl;
        close(serverSocket);
        return false;
    }
    
    // Set server socket to non-blocking mode
    int flags = fcntl(serverSocket, F_GETFL, 0);
    fcntl(serverSocket, F_SETFL, flags | O_NONBLOCK);
    
    running = true;
    std::cout << "Server started on port " << port << std::endl;
    return true;
}

bool NetworkServer::acceptClient() {
    // Don't try to accept if we already have a client
    if (clientSocket >= 0) {
        return true;
    }
    
    // Don't try to accept if server socket is invalid
    if (serverSocket < 0) {
        return false;
    }
    
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    
    int newSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
    if (newSocket < 0) {
        // EAGAIN/EWOULDBLOCK means no client is waiting (normal for non-blocking)
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return false;
        }
        // Other errors - only log once
        return false;
    }
    
    clientSocket = newSocket;
    
    // Set client socket to non-blocking mode
    int flags = fcntl(clientSocket, F_GETFL, 0);
    fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK);
    
    std::cout << "Client connected from " << inet_ntoa(clientAddr.sin_addr) << std::endl;
    return true;
}

bool NetworkServer::readCommand(std::string& command) {
    if (clientSocket < 0) return false;
    
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    
    // Non-blocking read - returns immediately if no data
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, MSG_DONTWAIT);
    if (bytesRead <= 0) {
        // No data available (EAGAIN/EWOULDBLOCK) or connection closed
        if (bytesRead == 0 || (errno != EAGAIN && errno != EWOULDBLOCK)) {
            // Connection closed
            return false;
        }
        // No data yet, but connection still alive
        return false;
    }
    
    command = std::string(buffer, bytesRead);
    // Remove trailing newline if present
    if (!command.empty() && command[command.length()-1] == '\n') {
        command = command.substr(0, command.length()-1);
    }
    
    return true;
}

void NetworkServer::sendResponse(const std::string& response) {
    if (clientSocket < 0) return;
    
    std::string data = response + "\n";
    send(clientSocket, data.c_str(), data.length(), 0);
}

bool NetworkServer::isClientConnected() const {
    return clientSocket >= 0;
}

void NetworkServer::stop() {
    running = false;
    if (clientSocket >= 0) {
        close(clientSocket);
        clientSocket = -1;
    }
    if (serverSocket >= 0) {
        close(serverSocket);
        serverSocket = -1;
    }
}
