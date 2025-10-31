#ifndef STAFFCHAINHANDLER_H
#define STAFFCHAINHANDLER_H

#include "Command.h"
#include "PlantProduct.h"
#include <chrono>
#include <string>
#include <thread>

class StaffMember; // Forward declaration

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
    StaffMember* manager; // Back-pointer to the dispatcher
    PlantProduct* activePlant = nullptr;
    std::string activeTask;
    bool busy;

public:
    StaffChainHandler() : next(nullptr), manager(nullptr), busy(false) {}
    virtual ~StaffChainHandler() {}

    void setNext(StaffChainHandler* next) { this->next = next; }
    void setManager(StaffMember* mgr) { this->manager = mgr; } // Setter for the manager
    bool isBusy() const { return activePlant != nullptr; }
    void setBusy(bool status) { this->busy = status; }
    const PlantProduct* getActivePlant() const { return activePlant; }
    std::string getActiveTask() const { return activeTask; }

    void clearAssignment() {
        activePlant = nullptr;
        activeTask.clear();
    }

    virtual void setBusyFor(std::chrono::seconds duration) {
        busy = true;
        std::thread([this, duration]() {
            std::this_thread::sleep_for(duration);
            this->busy = false;
            this->clearAssignment();
        }).detach();
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