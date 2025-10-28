#ifndef STAFFCHAINHANDLER_H
#define STAFFCHAINHANDLER_H

#include "Command.h"
#include "PlantProduct.h"
#include <iostream>
<<<<<<< HEAD
=======
#include <chrono>
#include <thread>
>>>>>>> a5cc157f06bf6b3c50e503d2c0883ab9968a4990

class StaffChainHandler {
protected:
    StaffChainHandler* next;
<<<<<<< HEAD
=======
    PlantProduct* activePlant = nullptr;
>>>>>>> a5cc157f06bf6b3c50e503d2c0883ab9968a4990
    bool busy;

public:
    StaffChainHandler() : next(nullptr), busy(false) {}
    virtual ~StaffChainHandler() {}

    void setNext(StaffChainHandler* next) { this->next = next; }
<<<<<<< HEAD
    bool isBusy() const { return busy; }
    void setBusy(bool status) { this->busy = status; }
=======
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
>>>>>>> a5cc157f06bf6b3c50e503d2c0883ab9968a4990

    /**
     * @brief Handles a command or passes it to the next handler in the chain.
     */
    virtual void handleCommand(Command* command) = 0;
};

#endif // STAFFCHAINHANDLER_H
