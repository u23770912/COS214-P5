#ifndef GARDENER_H
#define GARDENER_H

#include "GreenhouseStaff.h"
#include <iostream>

/**
 * @class Gardener
 * @brief A Concrete Handler for greenhouse tasks.
 * 
 * This is an instantiable class. The chain of responsibility for the
 * greenhouse team will be composed of instances of Gardener.
 */
class Gardener : public GreenhouseStaff {
public:
    void handleCommand(CareCommand* command) override {
        if (!isBusy()) {
            std::cout << "Gardener is handling the '" << command->getType() << "' command." << std::endl;
            setBusy(true);
            command->execute();
            setBusy(false);
            delete command;
        } else if (nextInChain != nullptr) {
            std::cout << "Gardener is busy, passing to next in the greenhouse team." << std::endl;
            nextInChain->handleCommand(command);
        } else {
            std::cout << "All Gardeners are busy. The '" << command->getType() << "' task was dropped. Plant will wither." << std::endl;
            command->getReceiver()->transitionToWithering();
            delete command;
        }
    }
};

#endif // GARDENER_H
