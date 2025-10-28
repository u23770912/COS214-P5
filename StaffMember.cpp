#include "StaffMember.h"
#include "CareCommand.h"
#include "PlantProduct.h"



void StaffMember::dispatch(Command* command) {
    if (!command) {
        std::cout << "Error: Received a null command." << std::endl;
        return;
    }

    std::string role = command->getRequiredRole();
    auto it = teams.find(role);

    if (it != teams.end()) {
        std::cout << "Dispatching command '" << command->getType() << "' to the '" << role << "' team." << std::endl;
        it->second->handleCommand(command);
    } else {
        std::cout << "System Error: No team registered for role '" << role << "'. Command dropped." << std::endl;
        
        // Attempt to transition the plant to withering if it's a CareCommand
        if (CareCommand* careCommand = dynamic_cast<CareCommand*>(command)) {
            if (careCommand->getReceiver()) {
                careCommand->getReceiver()->transitionToWithering();
            }
        }
        delete command;
    }
}
