#include "AutonomousMode.h"

void AutonomousMode::processUpdate(StaffManager* manager, PlantProduct* plant, const std::string& commandType) {
    std::cout << "StaffManager (AUTONOMOUS): Received notification for '" << commandType << "'. Creating and dispatching command." << std::endl;
    
    // Try Command factory first (for MoveToSalesFloorCommand, etc.)
    Command* command = Command::createCommand(commandType);
    
    // If not found, try CareCommand factory (for Watering, Pruning, Fertilizing)
    if (!command) {
        command = CareCommand::createCommand(commandType);
    }
    
    if (command) {
        command->setReceiver(plant);
        manager->dispatchCommand(command);
    } else {
        std::cout << "StaffManager Error: Could not create command for type '" << commandType << "'." << std::endl;
    }
}