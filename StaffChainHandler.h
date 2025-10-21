#ifndef STAFFCHAINHANDLER_H
#define STAFFCHAINHANDLER_H

#include "CareCommand.h"
#include "PlantProduct.h"
#include <iostream>

class StaffChainHandler {
protected:
    StaffChainHandler* next;
    bool busy;

public:
    StaffChainHandler() : next(nullptr), busy(false) {}
    virtual ~StaffChainHandler() {}

    void setNext(StaffChainHandler* next) { this->next = next; }
    bool isBusy() const { return busy; }
    void setBusy(bool status) { this->busy = status; }

    /**
     * @brief Handles a command or passes it to the next handler in the chain.
     */
    virtual void handleCommand(CareCommand* command) = 0;
};

#endif // STAFFCHAINHANDLER_H
