#ifndef ORDERPROCESSHANDLER_H
#define ORDERPROCESSHANDLER_H

#include "Order.h"
#include "Customer.h"
#include <string>
#include <iostream>

/**
 * @brief Abstract base class for order processing chain of responsibility
 * This implements the Chain of Responsibility pattern for order processing workflow
 */
class OrderProcessHandler {
protected:
    OrderProcessHandler* nextHandler;
    std::string handlerName;
    
public:
    OrderProcessHandler(const std::string& name) : nextHandler(nullptr), handlerName(name) {}
    virtual ~OrderProcessHandler() {}
    
    // Chain management
    void setNext(OrderProcessHandler* handler) {
        nextHandler = handler;
    }
    
    OrderProcessHandler* getNext() const {
        return nextHandler;
    }
    
    // Main processing method
    bool handleOrder(Order* order, Customer* customer) {
        std::cout << "\n--- " << handlerName << " ---" << std::endl;
        
        if (processOrder(order, customer)) {
            std::cout << "[SUCCESS] " << handlerName << " completed successfully" << std::endl;
            
            // Pass to next handler in chain
            if (nextHandler) {
                return nextHandler->handleOrder(order, customer);
            }
            return true; // End of chain, success
        } else {
            std::cout << "[FAILED] " << handlerName << " failed" << std::endl;
            return false; // Stop processing on failure
        }
    }
    
protected:
    // Abstract method to be implemented by concrete handlers
    virtual bool processOrder(Order* order, Customer* customer) = 0;
    
    // Utility method for logging
    void logStep(const std::string& message) {
        std::cout << "[" << handlerName << "] " << message << std::endl;
    }
};

#endif