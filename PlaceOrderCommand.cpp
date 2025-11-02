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
    
    // ============================================================
    // CHAIN OF RESPONSIBILITY PATTERN IMPLEMENTATION
    // ============================================================
    // Build the chain based on validation result:
    // 
    // Success Path: Validation → Payment → Notification (success)
    // Failure Path: Validation → Notification (failure)
    // ============================================================
    
    // Step 1: Create OrderValidationHandler (first in chain)
    OrderValidationHandler* validator = new OrderValidationHandler();
    
    // Step 2: Validate the order
    std::cout << "\n[CHAIN] Starting order processing chain..." << std::endl;
    bool validationSuccess = validator->handleOrder(order, customer);
    
    if (validationSuccess) {
        // ============================================================
        // SUCCESS PATH: Validation passed
        // Now chain: Payment → Notification (success)
        // ============================================================
        std::cout << "\n[CHAIN] Validation successful - proceeding to payment..." << std::endl;
        
        // Create PaymentProcessHandler
        PaymentProcessHandler* paymentHandler = new PaymentProcessHandler();
        
        // Create success NotificationHandler
        NotificationHandler* notificationHandler = new NotificationHandler(false); // false = success notification
        
        // Chain them: Payment → Notification
        paymentHandler->setNext(notificationHandler);
        
        // Execute the payment chain
        bool paymentSuccess = paymentHandler->handleOrder(order, customer);
        
        if (paymentSuccess) {
            executed = true;
            order->setStatus("Completed - Paid");
            std::cout << "\n[CHAIN] ✓ Order processing chain completed successfully!" << std::endl;
        } else {
            order->setStatus("Payment Failed");
            std::cout << "\n[CHAIN] ✗ Payment failed in chain!" << std::endl;
        }
        
        // Cleanup
        delete paymentHandler; // This will also delete notificationHandler (chain cleanup)
        
    } else {
        // ============================================================
        // FAILURE PATH: Validation failed
        // Chain: Notification (failure)
        // ============================================================
        std::cout << "\n[CHAIN] Validation failed - sending failure notification..." << std::endl;
        
        // Create failure NotificationHandler
        NotificationHandler* failureNotification = new NotificationHandler(true); // true = failure notification
        failureNotification->setErrorMessages(validator->getValidationErrors());
        
        // Process failure notification
        failureNotification->handleOrder(order, customer);
        
        order->setStatus("Validation Failed");
        std::cout << "\n[CHAIN] ✗ Order validation failed!" << std::endl;
        
        // Cleanup
        delete failureNotification;
    }
    
    // Cleanup validator
    delete validator;
    
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
