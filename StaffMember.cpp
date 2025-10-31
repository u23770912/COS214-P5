#include "StaffMember.h"
#include "Command.h"
#include "PlantProduct.h"

void StaffMember::queueUnhandledCommand(Command* command) {
    if (command) {
        std::cout << "Queueing unhandled command '" << command->getType() << "' for later." << std::endl;
        unhandledCommands.push(command);
    }
}

void StaffMember::processUnhandledQueue() {
    if (!unhandledCommands.empty()) {
        Command* command = unhandledCommands.front();
        unhandledCommands.pop();
        std::cout << "Re-dispatching command '" << command->getType() << "' from the unhandled queue." << std::endl;
        dispatch(command);
    }
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
        
        // Attempt to transition the plant to withering if the command has a plant receiver
        if (command->getReceiver()) {
            command->getReceiver()->transitionToWithering();
        }
        delete command;
    }
}