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
                     << " has been confirmed! Total: R" << std::fixed << std::setprecision(2) 
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
        logStep("Sending failure notification email to: " + customer->getEmail());
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "[EMAIL NOTIFICATION - ORDER ISSUE]" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << notification << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        
        // Send SMS if phone available
        if (!customer->getCellPhone().empty()) {
            logStep("Sending SMS notification to: " + customer->getCellPhone());
            std::cout << "\n[SMS]: Your order " << order->getOrderId() 
                     << " could not be processed. Please check your email for details." << std::endl;
        }
        
        // Update order status
        order->setStatus("Failed - Customer Notified");
        logStep("Failure notification sent to customer");
        
        return true; // Notification sent successfully even though order failed
    }
    
    std::string generateFailureNotification(Order* order, Customer* customer) {
        std::ostringstream notification;
        
        notification << "Dear " << customer->getName() << ",\n\n";
        notification << "We're sorry, but we encountered an issue with your order.\n\n";
        notification << "ORDER DETAILS:\n";
        notification << "Order ID: " << order->getOrderId() << "\n";
        notification << "Order Date: " << order->getOrderDate() << "\n";
        notification << "Status: " << order->getStatus() << "\n\n";
        
        notification << "ISSUE(S) ENCOUNTERED:\n";
        if (errorMessages.empty()) {
            notification << "- An unexpected error occurred during order processing.\n";
        } else {
            for (size_t i = 0; i < errorMessages.size(); i++) {
                notification << (i+1) << ". " << errorMessages[i] << "\n";
            }
        }
        
        notification << "\nWHAT YOU CAN DO:\n";
        notification << "- Modify your order and try again\n";
        notification << "- Contact our staff for assistance\n";
        notification << "- Check our website for updated availability\n\n";
        
        notification << "If you need immediate assistance, please contact us:\n";
        notification << "Phone: (555) 123-4567\n";
        notification << "Email: support@greengarden.com\n\n";
        
        notification << "We apologize for any inconvenience.\n\n";
        notification << "Best regards,\n";
        notification << "The Green Garden Team";
        
        return notification.str();
    }
    
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
        
        confirmation << "TOTAL AMOUNT: R" << std::fixed << std::setprecision(2) 
                    << order->getTotalAmount() << "\n\n";
        
        confirmation << "Your plants are ready for pickup or will be prepared for delivery.\n";
        confirmation << "Thank you for choosing Green Garden Nursery!\n\n";
        confirmation << "Best regards,\n";
        confirmation << "The Green Garden Team";
        
        return confirmation.str();
    }
};

#endif