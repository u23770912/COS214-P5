#include "Cashier.h"
#include "CareCommand.h"
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
        
        // Only transition to withering for care commands that affect plant health
        if (CareCommand* careCmd = dynamic_cast<CareCommand*>(command)) {
            if (careCmd->getReceiver()) {
                std::cout << "Plant will wither due to neglected care." << std::endl;
                careCmd->getReceiver()->transitionToWithering();
            }
        }
        // For MoveToSalesFloorCommand, we don't wither the plant
        // but we do need to handle the situation
        else if (MoveToSalesFloorCommand* moveCmd = dynamic_cast<MoveToSalesFloorCommand*>(command)) {
            std::cout << "Warning: Plant could not be moved to sales floor - will retry later." << std::endl;
        }
        
        delete command;
    }
}
