#include "PlaceOrderCommand.h"
#include "Order.h"
#include "Customer.h"
#include "OrderValidationHandler.h"
#include "PaymentProcessHandler.h"
#include "NotificationHandler.h"
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
    
    std::cout << "\n╔════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║         EXECUTING ORDER THROUGH CHAIN OF RESPONSIBILITY   ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════╝\n" << std::endl;
    std::cout << "Customer: " << customer->getName() << std::endl;
    std::cout << "Order ID: " << order->getOrderId() << std::endl;
    std::cout << "Timestamp: " << timestamp << std::endl;
        
    // Step 1: Create all handlers
    OrderValidationHandler* validator = new OrderValidationHandler();
    PaymentProcessHandler* paymentHandler = new PaymentProcessHandler();
    NotificationHandler* successNotification = new NotificationHandler(false); // success
    NotificationHandler* failureNotification = new NotificationHandler(true);  // failure
    
    // Step 2: Build the complete chain
    // Validation → Payment → Success Notification
    validator->setNext(paymentHandler);
    paymentHandler->setNext(successNotification);
    
    std::cout << "\n[CHAIN] Starting order processing chain..." << std::endl;
    std::cout << "[CHAIN] Chain structure: Validation → Payment → Notification" << std::endl;
    
    // Step 3: Start the chain with validation
    bool validationSuccess = validator->handleOrder(order, customer);
    
    if (validationSuccess) {
        std::cout << "\n[CHAIN] Full chain completed successfully!" << std::endl;
        executed = true;
        order->setStatus("Completed - Paid");
    } else {

        std::cout << "\n[CHAIN] Validation failed - sending failure notification..." << std::endl;
        
    
        failureNotification->setErrorMessages(validator->getValidationErrors());
        failureNotification->handleOrder(order, customer);
        
        order->setStatus("Validation Failed");
        std::cout << "\n[CHAIN] Order validation failed!" << std::endl;
    }
    
    // Cleanup all handlers
    delete validator;
    delete paymentHandler;
    delete successNotification;
    delete failureNotification;
    
    std::cout << "\n╔════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║              CHAIN OF RESPONSIBILITY COMPLETE              ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════╝\n" << std::endl;
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
