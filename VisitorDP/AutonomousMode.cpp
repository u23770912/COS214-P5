#include "AutonomousMode.h"

void AutonomousMode::processUpdate(StaffManager* manager, PlantProduct* plant, const std::string& commandType) {
    std::cout << "StaffManager (AUTONOMOUS): Received notification for '" << commandType << "'. Creating and dispatching command." << std::endl;
    Command* command = Command::createCommand(commandType);
    if (command) {
        manager->dispatchCommand(command);
    } else {
        std::cout << "StaffManager Error: Could not create command for type '" << commandType << "'." << std::endl;
    }
}

void AutonomousMode::resolvePendingTask(StaffManager* manager, const std::string& userInput) {
    std::cout << "StaffManager (AUTONOMOUS): No pending tasks to resolve." << std::endl;
    // Does nothing in autonomous mode
}
