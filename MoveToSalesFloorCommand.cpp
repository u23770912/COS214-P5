#include "MoveToSalesFloorCommand.h"
#include "PlantProduct.h"
#include "ReadyForSaleState.h"
#include "InventoryManager.h"
#include <iostream>

MoveToSalesFloorCommand::MoveToSalesFloorCommand() 
    : Command() {
    plantReceiver = nullptr;
}

MoveToSalesFloorCommand::~MoveToSalesFloorCommand() {
    // Don't delete plantReceiver - we don't own it
}

void MoveToSalesFloorCommand::setReceiver(PlantProduct* plant) {
    Command::setReceiver(plant);  // Use base class implementation
}

void MoveToSalesFloorCommand::execute() {
    if (!plantReceiver) {
        std::cout << "Error: No plant receiver set for MoveToSalesFloorCommand." << std::endl;
        return;
    }
    
    std::cout << "Executing MoveToSalesFloorCommand for plant..." << std::endl;
    
    // Check if the plant is in ReadyForSaleState
    if (plantReceiver->getCurrentStateName() != "ReadyForSale") {
        std::cout << "Plant is not ready for sale (Current state: " 
                  << plantReceiver->getCurrentStateName() 
                  << "). Cannot move to sales floor." << std::endl;
        return;
    }
    
    std::cout << "Plant is ready for sale. Moving to sales floor inventory..." << std::endl;
    
    // Get the inventory manager (Singleton) - using reference
    InventoryManager& inventory = InventoryManager::getInstance();
    
    // Move the plant from greenhouse to sales floor
    if (inventory.isPlantInGreenhouse(plantReceiver)) {
        inventory.removeFromGreenhouse(plantReceiver);
        inventory.moveToSalesFloor(plantReceiver);
        std::cout << "Plant successfully moved to sales floor inventory!" << std::endl;
    } else {
        std::cout << "Warning: Plant was not found in greenhouse inventory." << std::endl;
        std::cout << "Adding directly to sales floor..." << std::endl;
        inventory.moveToSalesFloor(plantReceiver);
    }
}

std::string MoveToSalesFloorCommand::getType() const {
    return "MoveToSalesFloor";
}

std::string MoveToSalesFloorCommand::getRequiredRole() const {
    return "Sales";
}

Command* MoveToSalesFloorCommand::clone() const {
    MoveToSalesFloorCommand* cloned = new MoveToSalesFloorCommand();
    cloned->setReceiver(this->plantReceiver);  // Use setter to properly set base class member
    return cloned;
}
