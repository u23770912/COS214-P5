#include "Gardener.h"
#include "../CommandDP/Command.h"
#include "../CommandDP/CareCommand.h"
#include "../PlantProduct.h"
#include <iostream>

// The handleCommand logic is already in the header, so this file is for completeness.
// If the logic were more complex, it would be moved here.

// Example of how it would look if moved:
/*
void Gardener::handleCommand(Command* command) {
    if (!isBusy()) {
        std::cout << "Gardener is handling the '" << command->getType() << "' command." << std::endl;
        setBusy(true);
        command->execute();
        setBusy(false);
        delete command;
    } else if (next != nullptr) {
        std::cout << "Gardener is busy, passing to next in the greenhouse team." << std::endl;
        next->handleCommand(command);
    } else {
        std::cout << "All Gardeners are busy. The '" << command->getType() << "' task was dropped." << std::endl;
        if (CareCommand* careCmd = dynamic_cast<CareCommand*>(command)) {
            if (careCmd->getReceiver()) {
                careCmd->getReceiver()->transitionToWithering();
            }
        }
        delete command;
    }
}
*/
