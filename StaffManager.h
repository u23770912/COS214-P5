// ============= StaffManager.h =============
#ifndef STAFFMANAGER_H
#define STAFFMANAGER_H

#include "LifecycleObserver.h"
#include "CustomerObserver.h"
#include "ModeVisitor.h"
#include <string>
#include <iostream>

class StaffMember;
class PlantProduct;
class Command;
class Order;
class Customer;

/**
 * @class StaffManager
 * @brief ConcreteObserver for both Plant Lifecycle and Customer interactions
 * Observes plants (LifeCycleObserver) and customers (CustomerObserver)
 * Delegates behavior to a ModeVisitor
 */
class StaffManager : public LifeCycleObserver, public CustomerObserver {
private:
    StaffMember* staffDispatcher;
    ModeVisitor* currentModeVisitor;
    
public:
    StaffManager(StaffMember* dispatcher);
    ~StaffManager();

    void setMode(ModeVisitor* newMode);
    
    // LifeCycleObserver interface - for plant lifecycle events
    void update(PlantProduct* plant, const std::string& commandType) override;
    
    // CustomerObserver interface - for customer interaction events
    void updateCustomerInteraction(Customer* customer, 
                                   const std::string& interactionType, 
                                   const std::string& details = "") override;
    bool validateCustomerOrder(Order* order, Customer* customer) override;
    
    void dispatchCommand(Command* command);
};

#endif // STAFFMANAGER_H