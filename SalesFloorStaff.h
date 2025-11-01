#ifndef SALESFLOORSTAFF_H
#define SALESFLOORSTAFF_H

#include "StaffChainHandler.h"

/**
 * @class SalesFloorStaff
 * @brief An abstract handler representing the sales floor department/role.
 * 
 * This class serves as the base for all concrete workers on the sales floor,
 * such as SalesAssociates.
 */
class SalesFloorStaff : public StaffChainHandler {
    public:
        SalesFloorStaff() : StaffChainHandler() {}
        virtual ~SalesFloorStaff() {}

        void setNext(StaffChainHandler* next) { this->next = next; }
        bool isBusy() const { return busy; }
        void setBusy(bool status) { this->busy = status; }

        /**
         * @brief Handles a command or passes it to the next handler in the chain.
         */
        virtual void handleCommand(Command* command) = 0;
};

#endif // SALESFLOORSTAFF_H
