#ifndef PLACEORDERCOMMAND_H
#define PLACEORDERCOMMAND_H

#include <string>

class Order; // Forward declaration
class Customer; // Forward declaration

/**
 * @brief Command class for placing orders (Command pattern)
 * Encapsulates the action of placing an order
 */
class PlaceOrderCommand {
private:
    Order* order;
    Customer* customer;
    std::string timestamp;
    bool executed;

public:
    PlaceOrderCommand(Order* order, Customer* customer);
    ~PlaceOrderCommand();
    
    // Command interface
    void execute();
    void undo();
    
    // Getters
    Order* getOrder() const;
    Customer* getCustomer() const;
    std::string getTimestamp() const;
    bool isExecuted() const;
    
    // Command information
    std::string getCommandInfo() const;

private:
    // Helper method to generate timestamp
    std::string generateTimestamp();
};

#endif