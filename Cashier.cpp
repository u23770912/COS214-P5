#include "Cashier.h"
#include "Command.h"
#include "MoveToSalesFloorCommand.h"
#include <iostream>

void Cashier::handleCommand(Command* command) {
    if (!command) {
        std::cout << "Cashier received null command." << std::endl;
        return;
    }
    
    // Check if this cashier is available
    if (!isBusy()) {
        std::cout << "Cashier is handling the '" << command->getType() << "' command." << std::endl;
        setBusy(true);
        
        // Execute the command
        command->execute();
        
        setBusy(false);
        delete command;
    } 
    // If busy, pass to next in chain
    else if (next != nullptr) {
        std::cout << "Cashier is busy, passing to next in the sales team." << std::endl;
        next->handleCommand(command);
    } 
    // No one available in the chain
    else {
        std::cout << "All Cashiers are busy. The '" << command->getType() << "' task was dropped." << std::endl;
        
        // Check if command has a plant receiver (care commands that affect plant health)
        if (command->getReceiver()) {
            // Only wither if it's not a MoveToSalesFloorCommand
            if (dynamic_cast<MoveToSalesFloorCommand*>(command) == nullptr) {
                std::cout << "Plant will wither due to neglected care." << std::endl;
                command->getReceiver()->transitionToWithering();
            } else {
                std::cout << "Warning: Plant could not be moved to sales floor - will retry later." << std::endl;
            }
        }
        
        delete command;
    }
}
