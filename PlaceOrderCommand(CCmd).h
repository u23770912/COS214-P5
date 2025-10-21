#ifndef PLACEORDERCOMMAND_H
#define PLACEORDERCOMMAND_H

#include "OrderCommand(Interface).h"
#include "Customer.h"

class StaffManager;
class StaffMember;
class Order;

class PlaceOrderCommand : public OrderCommand {

    private:
        std::string requiredRole = "Sales";
        StaffManager* staffManager; //will be used to get the correct staff member to process the order
        Order* order;

    public:

        void execute() override {

            // StaffMember* staffMember = getStaffMember();
            // staffMember->processOrder();
        }

        StaffMember* getStaffMember() const {
            // return staffManager->dispatchIntoChain(this, this->getRequiredRole());
        }

        std::string getRequiredRole() const {
            return requiredRole;
        }
};

#endif