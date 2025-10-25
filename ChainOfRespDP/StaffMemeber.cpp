#include "StaffMember.h"
#include "../CommandDP/Command.h"
#include "../PlantProduct.h"

// The dispatch logic is already in the header, so this file is for completeness.
// If the dispatch logic were more complex, it would be moved here.
// For now, we can add the destructor implementation.

StaffMember::~StaffMember() {
    // A real application would need to carefully manage the memory of the
    // team handlers. If StaffMember owns them, it should delete them.
    // For this simulation, we assume ownership is managed elsewhere.
    std::cout << "StaffMember dispatcher is being destroyed." << std::endl;
}

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
        
        // TODO: Attempt to transition the plant to withering if it's a CareCommand
        // This code is commented out because CareCommand doesn't exist yet
        /*
        if (CareCommand* careCommand = dynamic_cast<CareCommand*>(command)) {
            if (careCommand->getReceiver()) {
                careCommand->getReceiver()->transitionToWithering();
            }
        }
        */
        delete command;
    }
}