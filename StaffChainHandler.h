#ifndef STAFFCHAINHANDLER_H
#define STAFFCHAINHANDLER_H

#include "Command.h"
#include "PlantProduct.h"
#include <iostream>
#include <chrono>
#include <thread>

class StaffChainHandler {
protected:
    StaffChainHandler* next;
    PlantProduct* activePlant = nullptr;
    bool busy;

public:
    StaffChainHandler() : next(nullptr), busy(false) {}
    virtual ~StaffChainHandler() {}

    void setNext(StaffChainHandler* next) { this->next = next; }
    bool isBusy() const { return activePlant != nullptr; }
    void setBusy(bool status) { this->busy = status; }
    virtual void setBusyFor(std::chrono::seconds duration) {
        busy = true;
        std::thread([this, duration]() {
            std::this_thread::sleep_for(duration);
            this->busy = false;
            this->activePlant = nullptr;
        }).detach();
    }

    /**
     * @brief Handles a command or passes it to the next handler in the chain.
     */
    virtual void handleCommand(Command* command) = 0;
};

#endif // STAFFCHAINHANDLER_H
