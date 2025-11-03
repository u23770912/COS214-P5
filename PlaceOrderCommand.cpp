#include "PlaceOrderCommand.h"
#include "Order.h"
#include "Customer.h"
#include <iostream>
#include <ctime>

PlaceOrderCommand::PlaceOrderCommand(Order* order, Customer* customer)
    : order(order), customer(customer), executed(false), salesStaff(nullptr) {
    timestamp = generateTimestamp();
}

PlaceOrderCommand::~PlaceOrderCommand() {
    // Don't delete order or customer - we don't own them
}

void PlaceOrderCommand::execute() {
    if (executed) {
        std::cout << "Order has already been executed." << std::endl;
        return;
    }
    
    if (!order || !customer) {
        std::cout << "Error: Invalid order or customer." << std::endl;
        return;
    }
    
    std::cout << "\n=== Executing PlaceOrderCommand ===" << std::endl;
    std::cout << "Customer: " << customer->getName() << std::endl;
    std::cout << "Order ID: " << order->getOrderId() << std::endl;
    std::cout << "Timestamp: " << timestamp << std::endl;
    
    // For now, just mark as executed
    // In full implementation, this would process through validation chain
    executed = true;
    order->setStatus("Placed");
    
    std::cout << "Order placed successfully!" << std::endl;
    std::cout << "==================================\n" << std::endl;
}

Order* PlaceOrderCommand::getOrder() const {
    return order;
}

Customer* PlaceOrderCommand::getCustomer() const {
    return customer;
}

std::string PlaceOrderCommand::getTimestamp() const {
    return timestamp;
}

bool PlaceOrderCommand::isExecuted() const {
    return executed;
}

std::string PlaceOrderCommand::getCommandInfo() const {
    return "PlaceOrderCommand for Order " + order->getOrderId() + 
           " by " + customer->getName();
}

std::string PlaceOrderCommand::generateTimestamp() {
    time_t now = time(0);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return std::string(buf);
}

void PlaceOrderCommand::setReceiver(PlantProduct* plant) {
    // PlaceOrderCommand doesn't need a plant receiver
    (void)plant; // Suppress unused parameter warning
}
