#ifndef CUSTOMERSUBJECT_H
#define CUSTOMERSUBJECT_H

#include <vector>
#include <string>
#include "CustomerObserver.h"

// Forward declarations
class Order;

/**
 * @brief Abstract Subject for the Customer Observer pattern
 * Maintains a list of observers and notifies them of customer events
 */
class CustomerSubject {
protected:
    std::vector<CustomerObserver*> observers;
    
public:
    virtual ~CustomerSubject() = default;
    
    /**
     * @brief Attach an observer to this subject
     * @param observer The observer to attach
     */
    virtual void attach(CustomerObserver* observer) {
        if (observer) {
            observers.push_back(observer);
        }
    }
    
    /**
     * @brief Detach an observer from this subject
     * @param observer The observer to detach
     */
    virtual void detach(CustomerObserver* observer) {
        for (auto it = observers.begin(); it != observers.end(); ++it) {
            if (*it == observer) {
                observers.erase(it);
                break;
            }
        }
    }
    
    /**
     * @brief Notify all observers of a customer interaction
     * @param interactionType Type of interaction
     * @param details Additional details
     */
    virtual void notifyInteraction(const std::string& interactionType, 
                                   const std::string& details = "") = 0;
    
    /**
     * @brief Request validation from observers
     * @param order The order to validate
     * @return true if any observer validates successfully
     */
    virtual bool requestValidation(Order* order) = 0;
};

#endif
