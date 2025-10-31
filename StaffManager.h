#ifndef STAFFMANAGER_H
#define STAFFMANAGER_H

#include "LifeCycleObserver.h"
#include <string>
#include <iostream>

class StaffMember;
class PlantProduct;
class Command;
class Order;
class Customer;

/**
 * @class StaffManager
 * @brief The "Element" class that accepts a visitor to change its mode.
 * Observes plants, creates commands, and passes them to a dispatcher.
 */
class StaffManager : public LifeCycleObserver, public CustomerObserver {
    
    private:
        StaffMember* staffDispatcher;

        //Interactive and automated mode have been removed from system so we need to fix all automated and interactive mode fucntionality
        PlantProduct* pendingPlant;
        std::string expectedCommandType;

    public:
        StaffManager(StaffMember* dispatcher);
        ~StaffManager();

        //visitor management removed

        // --- Core Methods ---
        void resolvePendingTask(const std::string& userInput);
        void update(PlantProduct* plant, const std::string& commandType) override;

        void updateCustomerInteraction(Customer* customer, const std::string& interactionType, const std::string& details = "") override;
        bool validateCustomerOrder(Order* order, Customer* customer) override;

        void dispatchCommand(Command* command);

};

#endif // STAFFMANAGER_H