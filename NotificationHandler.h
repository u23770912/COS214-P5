#ifndef NOTIFICATIONHANDLER_H
#define NOTIFICATIONHANDLER_H

#include "OrderProcessHandler.h"
#include <sstream>
#include <iomanip>

/**
 * @brief Concrete handler for customer notification
 * Sends order confirmation and updates to the customer
 */
class NotificationHandler : public OrderProcessHandler {
public:
    NotificationHandler() : OrderProcessHandler("Customer Notification") {}
    
protected:
    bool processOrder(Order* order, Customer* customer) override {
        logStep("Preparing order confirmation for: " + customer->getName());
        
        // Generate order confirmation
        std::string confirmation = generateOrderConfirmation(order, customer);
        
        // Simulate sending notification
        logStep("Sending confirmation email to: " + customer->getEmail());
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "[EMAIL CONFIRMATION]" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << confirmation << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        
        // Simulate SMS notification if phone number is available
        if (!customer->getCellPhone().empty()) {
            logStep("Sending SMS notification to: " + customer->getCellPhone());
            std::cout << "\n[SMS]: Your order " << order->getOrderId() 
                     << " has been confirmed! Total: $" << std::fixed << std::setprecision(2) 
                     << order->getTotalAmount() << std::endl;
        }
        
        // Update order status
        order->setStatus("Completed - Customer Notified");
        logStep("Order processing completed successfully!");
        
        return true;
    }
    
private:
    std::string generateOrderConfirmation(Order* order, Customer* customer) {
        std::ostringstream confirmation;
        
        confirmation << "Dear " << customer->getName() << ",\n\n";
        confirmation << "Thank you for your order at Green Garden Nursery!\n\n";
        confirmation << "ORDER DETAILS:\n";
        confirmation << "Order ID: " << order->getOrderId() << "\n";
        confirmation << "Customer: " << customer->getName() << "\n";
        confirmation << "Email: " << customer->getEmail() << "\n";
        confirmation << "Order Date: " << order->getOrderDate() << "\n\n";
        
        confirmation << "ITEMS ORDERED:\n";
        confirmation << order->getOrderSummary() << "\n";
        
        confirmation << "TOTAL AMOUNT: $" << std::fixed << std::setprecision(2) 
                    << order->getTotalAmount() << "\n\n";
        
        confirmation << "Your plants are ready for pickup or will be prepared for delivery.\n";
        confirmation << "Thank you for choosing Green Garden Nursery!\n\n";
        confirmation << "Best regards,\n";
        confirmation << "The Green Garden Team";
        
        return confirmation.str();
    }
};

#endif