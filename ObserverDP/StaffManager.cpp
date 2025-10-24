#include "StaffManager.h"
#include "../CommandDP/Command.h"
#include "../CommandDP/CareCommand.h"
#include "../PlantProduct.h"
#include <iostream>

// Note: As per the user's request, StaffManager is the "Manager" class.

void StaffManager::update(PlantProduct* plant, const std::string& commandType) {
    std::cout << "StaffManager received notification for '" << commandType << "' from a plant." << std::endl;

    // Use the static factory on the base Command class to create a command instance.
    Command* command = Command::createCommand(commandType);

    if (command) {
        // Safely downcast to a CareCommand to set the plant receiver.
        if (CareCommand* careCommand = dynamic_cast<CareCommand*>(command)) {
            careCommand->setReceiver(plant);
            std::cout << "Command created and receiver set. Dispatching to StaffMember." << std::endl;
            
            // Dispatch the generic command pointer to the StaffMember.
            staffDispatcher->dispatch(careCommand);
        } else {
            // Handle cases where a command is created that is not a CareCommand.
            std::cout << "Warning: Command '" << commandType << "' is not a CareCommand. Dispatching without a receiver." << std::endl;
            staffDispatcher->dispatch(command);
        }
    } else {
        std::cout << "Error: Could not create command of type '" << commandType << "'. The plant may wither." << std::endl;
        plant->transitionToWithering();
    }
}
