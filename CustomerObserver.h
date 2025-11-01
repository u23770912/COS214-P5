#ifndef CUSTOMEROBSERVER_H
#define CUSTOMEROBSERVER_H

#include <string>

// Forward declarations
class Customer;
class Order;

/**
 * @brief Abstract Observer for the Customer Observer pattern
 * Defines the interface for objects that should be notified of customer interactions
 */
class CustomerObserver {
public:
    virtual ~CustomerObserver() = default;
    
    /**
     * @brief Update method called when customer state changes or events occur
     * @param customer The customer triggering the notification
     * @param interactionType Type of interaction (e.g., "ORDER_SUBMITTED", "ASSISTANCE_NEEDED")
     * @param details Additional details about the interaction
     */
    virtual void updateCustomerInteraction(Customer* customer, 
                                          const std::string& interactionType, 
                                          const std::string& details = "") = 0;
    
    /**
     * @brief Handle order validation request from customer
     * @param order The order to validate
     * @param customer The customer who submitted the order
     * @return true if validation succeeds, false otherwise
     */
    virtual bool validateCustomerOrder(Order* order, Customer* customer) = 0;
};

#endif
