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

    /**
     * @brief Set the next handler in the chain
     */
    void setNext(StaffChainHandler* nextHandler) { 
        next = nextHandler; 
    }

    /**
     * @brief Get the next handler in the chain
     */
    StaffChainHandler* getNext() const {
        return next;
    }

    /**
     * @brief Set the manager reference for queueing unhandled commands
     */
    void setManager(StaffMember* mgr) { 
        manager = mgr; 
    }

    /**
     * @brief Check if this staff member is busy
     * Checks if there is an active plant assignment
     */
    bool isBusy() const {
        return busy || activePlant != nullptr;
    }

    /**
     * @brief Set the busy status of this staff member
     */
    void setBusy(bool status) {
        busy = status;
    }

    /**
     * @brief Get the currently active plant being worked on
     */
    const PlantProduct* getActivePlant() const { 
        return activePlant; 
    }

    /**
     * @brief Get the type of task currently being performed
     */
    std::string getActiveTask() const { 
        return activeTask; 
    }

    /**
     * @brief Clear the current assignment when task is complete
     */
    void clearAssignment() {
        activePlant = nullptr;
        activeTask.clear();
    }

    /**
     * @brief Set busy status for a specific duration (simulates task time)
     * @param duration How long the staff member will be busy
     */
    virtual void setBusyFor(std::chrono::seconds duration) {
        busy = true;
        std::thread([this, duration]() {
            std::this_thread::sleep_for(duration);
            this->busy = false;
            this->clearAssignment();
        }).detach();
    }

    /**
     * @brief Handle a command or pass it to the next handler in the chain
     * This is the core method of the Chain of Responsibility pattern
     * @param command The command to handle
     */
    virtual void handleCommand(Command* command) = 0;
};

#endif // STAFFCHAINHANDLER_H