#include "InteractiveMode.h"

void InteractiveMode::processUpdate(StaffManager* manager, PlantProduct* plant, const std::string& commandType) {
    std::cout << "StaffManager (INTERACTIVE): Received notification for '" << commandType << "'. Awaiting user input." << std::endl;
    manager->setPendingTask(plant, commandType);
}

void InteractiveMode::resolvePendingTask(StaffManager* manager, const std::string& userInput) {
    if (!manager->hasPendingTask()) {
        std::cout << "StaffManager: No pending task to resolve." << std::endl;
        return;
    }

    PlantProduct* plant = manager->getPendingPlant();
    std::string expectedCommandType = manager->getExpectedCommandType();

    std::string finalCommandType = userInput;
    if (userInput == "skip") {
        finalCommandType = expectedCommandType;
        std::cout << "StaffManager: 'skip' entered. Resolving with expected command: '" << finalCommandType << "'." << std::endl;
    }

    if (finalCommandType == expectedCommandType) {
        std::cout << "StaffManager: Correct action '" << finalCommandType << "' provided. Creating and dispatching command." << std::endl;
        CareCommand* command = CareCommand::createCommand(finalCommandType);
        if (command) {
            if (plant) {
                command->setReceiver(plant);
            }
            manager->dispatchCommand(command);
        } else {
             std::cout << "StaffManager Error: Could not create command for type '" << finalCommandType << "'." << std::endl;
        }
    } else {
        std::cout << "StaffManager: Incorrect action '" << userInput << "' provided. Expected '" << expectedCommandType << "'. Plant will wither." << std::endl;
        plant->transitionToWithering();
    }

    // Clear the pending task
    manager->clearPendingTask();
}
