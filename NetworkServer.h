#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include <string>
#include <vector>
#include <sstream>

// Forward declarations
class PlantProduct;
class StaffChainHandler;

/**
 * @brief Simple network server that runs in background thread
 * Handles GUI commands while main simulation runs
 */
class NetworkServer {
private:
    int serverSocket;
    int clientSocket;
    bool running;
    
public:
    NetworkServer();
    ~NetworkServer();
    
    /**
     * @brief Start server on specified port
     * @param port Port number to listen on
     * @return true if started successfully
     */
    bool start(int port);
    
    /**
     * @brief Accept incoming connection
     * @return true if client connected
     */
    bool acceptClient();
    
    /**
     * @brief Read command from client
     * @param command Output string for command
     * @return true if command read successfully
     */
    bool readCommand(std::string& command);
    
    /**
     * @brief Send response to client
     * @param response Response string to send
     */
    void sendResponse(const std::string& response);
    
    /**
     * @brief Check if client is connected
     */
    bool isClientConnected() const;
    
    /**
     * @brief Stop server and close connections
     */
    void stop();
};

#endif // NETWORKSERVER_H
