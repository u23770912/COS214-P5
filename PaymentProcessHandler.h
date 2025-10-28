#ifndef PAYMENTPROCESSHANDLER_H
#define PAYMENTPROCESSHANDLER_H

#include "OrderProcessHandler.h"
#include <random>
#include <iostream>

/**
 * @brief Concrete handler for payment processing
 * Simulates payment processing for the order
 */
class PaymentProcessHandler : public OrderProcessHandler {
private:
    std::mt19937 rng;
    
public:
    PaymentProcessHandler() : OrderProcessHandler("Payment Processing"), rng(std::random_device{}()) {}
    
protected:
    bool processOrder(Order* order, Customer* customer) override {
        logStep("Processing payment for customer: " + customer->getName());
        
        double totalAmount = order->calculateTotalAmount();
        logStep("Total amount: $" + std::to_string(totalAmount));
        
        // Simulate payment processing time
        logStep("Contacting payment gateway...");
        
        // Simulate payment validation (90% success rate)
        std::uniform_int_distribution<int> dist(1, 10);
        bool paymentSuccess = (dist(rng) <= 9);
        
        if (paymentSuccess) {
            logStep("Payment authorized and processed successfully");
            order->setStatus("Paid");
            
            // Update order with payment info
            logStep("Payment confirmation sent to customer");
            return true;
        } else {
            std::cout << "[ERROR] Payment failed - Card declined or insufficient funds" << std::endl;
            order->setStatus("Payment Failed");
            return false;
        }
    }
};

#endif