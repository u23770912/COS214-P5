#ifndef NOTIFICATIONHANDLER_H
#define NOTIFICATIONHANDLER_H

#include "OrderProcessHandler.h"
#include <sstream>
#include <iomanip>

/**
 * @brief Concrete handler for customer notification
 * Sends order confirmation and updates to the customer
 * Can be used for both success and failure notifications
 */
class NotificationHandler : public OrderProcessHandler {
private:
    bool isFailureNotification;
    std::vector<std::string> errorMessages;
    
public:
    NotificationHandler(bool isFailure = false) 
        : OrderProcessHandler("Customer Notification"), isFailureNotification(isFailure) {}
    
    void setErrorMessages(const std::vector<std::string>& errors) {
        errorMessages = errors;
    }
    
protected:
    bool processOrder(Order* order, Customer* customer) override {
        if (isFailureNotification) {
            return sendFailureNotification(order, customer);
        } else {
            return sendSuccessNotification(order, customer);
        }
    }
    
private:
    bool sendSuccessNotification(Order* order, Customer* customer) {
        logStep("Preparing order confirmation for: " + customer->getName());
        
        // Generate order confirmation
        std::string confirmation = generateOrderConfirmation(order, customer);
        
        // Simulate sending notification
        logStep("Sending email to: " + customer->getEmail());
        std::cout << "\n" << std::string(40, '=') << std::endl;
        std::cout << "[ORDER CONFIRMED]" << std::endl;
        std::cout << std::string(40, '=') << std::endl;
        std::cout << confirmation << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        
        // Simulate SMS notification if phone number is available
        if (!customer->getCellPhone().empty()) {
            logStep("Sending SMS to: " + customer->getCellPhone());
            std::cout << "\n[SMS]: Order " << order->getOrderId() 
                     << " confirmed. Total: R" << std::fixed << std::setprecision(2) 
                     << order->getTotalAmount() << std::endl;
        }
        
        // Update order status
        order->setStatus("Completed - Customer Notified");
        logStep("Order processing completed successfully!");
        
        return true;
    }
    
    bool sendFailureNotification(Order* order, Customer* customer) {
        logStep("Preparing order failure notification for: " + customer->getName());
        
        // Generate failure notification
        std::string notification = generateFailureNotification(order, customer);
        
        // Send email notification
        logStep("Sending email to: " + customer->getEmail());
        std::cout << "\n" << std::string(40, '=') << std::endl;
        std::cout << "[ORDER PROBLEM]" << std::endl;
        std::cout << std::string(40, '=') << std::endl;
        std::cout << notification << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        
        // Send SMS if phone available
        if (!customer->getCellPhone().empty()) {
            logStep("Sending SMS to: " + customer->getCellPhone());
            std::cout << "\n[SMS]: Order " << order->getOrderId() 
                     << " failed. Please contact us." << std::endl;
        }
        
        // Update order status
        order->setStatus("Failed - Customer Notified");
        logStep("Failure notification sent to customer");
        
        return true; // Notification sent successfully even though order failed
    }
    
    std::string generateFailureNotification(Order* order, Customer* customer) {
        std::ostringstream notification;
        
        notification << "Hello " << customer->getName() << ",\n\n";
        notification << "There was a problem with your order.\n\n";
        notification << "Order: " << order->getOrderId() << "\n";
        notification << "Date: " << order->getOrderDate() << "\n\n";
        
        notification << "Problem:\n";
        if (errorMessages.empty()) {
            notification << "- Order could not be processed\n";
        } else {
            for (size_t i = 0; i < errorMessages.size(); i++) {
                notification << "- " << errorMessages[i] << "\n";
            }
        }
        
        notification << "\nPlease visit us or contact staff for help.\n\n";
        notification << "Green Garden Nursery";
        
        return notification.str();
    }
    
    std::string generateOrderConfirmation(Order* order, Customer* customer) {
        std::ostringstream confirmation;
        
        confirmation << "Hello " << customer->getName() << ",\n\n";
        confirmation << "Your order is confirmed!\n\n";
        confirmation << "Order: " << order->getOrderId() << "\n";
        confirmation << "Date: " << order->getOrderDate() << "\n";
        confirmation << "Total: R" << std::fixed << std::setprecision(2) 
                    << order->getTotalAmount() << "\n\n";
        
        confirmation << order->getOrderSummary() << "\n";
        
        confirmation << "Your plants are ready for collection.\n";
        confirmation << "Thank you!\n\n";
        confirmation << "Green Garden Nursery";
        
        return confirmation.str();
    }
};

#endif