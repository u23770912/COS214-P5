#ifndef STAFFCHAINHANDLER_H
#define STAFFCHAINHANDLER_H

#include "Command.h"

/**
 * @class StaffChainHandler
 * @brief Abstract base class for the Chain of Responsibility pattern for staff members.
 * 
 * This class represents the Handler in the Chain of Responsibility pattern.
 * Each concrete staff member inherits from this class.
 */
class StaffChainHandler {
protected:
    StaffChainHandler* next;
    bool busy;

public:
    StaffChainHandler() : next(nullptr), busy(false) {}
    virtual ~StaffChainHandler() {
        delete next;
    }

    /**
     * @brief Set the next handler in the chain
     */
    virtual void setNext(StaffChainHandler* nextHandler) {
        next = nextHandler;
    }

    /**
     * @brief Get the next handler in the chain
     */
    StaffChainHandler* getNext() const {
        return next;
    }

    /**
     * @brief Check if this staff member is busy
     */
    bool isBusy() const {
        return busy;
    }

    /**
     * @brief Set the busy status of this staff member
     */
    void setBusy(bool status) {
        busy = status;
    }

    /**
     * @brief Handle a command or pass it to the next handler in the chain
     */
    virtual void handleCommand(Command* command) = 0;
};

#endif // STAFFCHAINHANDLER_H