#ifndef GREENHOUSESTAFF_H
#define GREENHOUSESTAFF_H

#include "StaffChainHandler.h"

class GreenhouseStaff : public StaffChainHandler
{
public:
    GreenhouseStaff() : StaffChainHandler() {}
    virtual ~GreenhouseStaff() {}

    void setNext(StaffChainHandler *next) { this->next = next; }
    bool isBusy() const { return busy; }
    void setBusy(bool status) { this->busy = status; }
    /**
     * @brief Handles a command or passes it to the next handler in the chain.
     */
    virtual void handleCommand(Command *command) = 0;
};

#endif // GREENHOUSESTAFF_H