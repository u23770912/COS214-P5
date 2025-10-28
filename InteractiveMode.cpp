#include "InteractiveMode.h"

void InteractiveMode::processUpdate(StaffManager* manager, PlantProduct* plant, const std::string& commandType) {
    std::cout << "StaffManager (INTERACTIVE): Received notification for '" << commandType << "'. Awaiting user input." << std::endl;
    
    // Store the task in THIS visitor's state
    pendingPlant = plant;
    expectedCommand = commandType;
    
    // Request user input
    std::cout << "Enter command (or 'skip'): ";
    std::string userInput;
    std::cin >> userInput;
    
    // Process the input immediately
    std::string finalCommand = userInput;
    if (userInput == "skip") {
        finalCommand = expectedCommand;
        std::cout << "StaffManager: 'skip' entered. Using expected command: '" << finalCommand << "'." << std::endl;
    }
    
    if (finalCommand == expectedCommand) {
        std::cout << "StaffManager: Correct action '" << finalCommand << "' provided. Creating and dispatching command." << std::endl;
        
        CareCommand* command = CareCommand::createCommand(finalCommand);
        if (command) {
            command->setReceiver(pendingPlant);
            manager->dispatchCommand(command);
        } else {
            std::cout << "StaffManager Error: Could not create command for type '" << finalCommand << "'." << std::endl;
        }
    } else {
        std::cout << "StaffManager: Incorrect action '" << userInput << "' provided. Expected '" << expectedCommand << "'. Plant will wither." << std::endl;
        pendingPlant->transitionToWithering();
    }
    
    // Clear state
    pendingPlant = nullptr;
    expectedCommand = "";
}