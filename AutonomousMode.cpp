#include "AutonomousMode.h"

void AutonomousMode::processUpdate(StaffManager* manager, PlantProduct* plant, const std::string& commandType) {
    std::cout << "StaffManager (AUTONOMOUS): Received notification for '" << commandType << "'. Creating and dispatching command." << std::endl;
    CareCommand* command = CareCommand::createCommand(commandType);
    if (command) {
        CareCommand* careCmd = dynamic_cast<CareCommand*>(command);
        if (careCmd && plant) {
            careCmd->setReceiver(plant);
        }
        manager->dispatchCommand(dynamic_cast<Command*>(command));
    }
}

void AutonomousMode::resolvePendingTask(StaffManager* manager, const std::string& userInput) {
    std::cout << "StaffManager (AUTONOMOUS): No pending tasks to resolve." << std::endl;
    // Does nothing in autonomous mode
}
