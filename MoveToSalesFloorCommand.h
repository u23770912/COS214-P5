#ifndef MOVE_TO_SALES_FLOOR_COMMAND_H
#define MOVE_TO_SALES_FLOOR_COMMAND_H

#include "Command.h"
#include "PlantProduct.h"
#include "StateDP/ReadyForSaleState.h"
#include "InventoryManager.h"
#include <iostream>

class MoveToSalesFloorCommand : public Command {
private:
    PlantProduct* plantReceiver;
    
public:
    // Constructor for prototype
    MoveToSalesFloorCommand() : Command(), plantReceiver(nullptr) {}
    
    // Set the plant receiver
    void setReceiver(PlantProduct* plant) { plantReceiver = plant; }
    void execute() override {
        if (plantReceiver) {
            std::cout << "Executing MoveToSalesFloorCommand for plant..." << std::endl;
            
            // Check if the plant is in ReadyForSaleState
            if (plantReceiver->getCurrentStateName() == "ReadyForSale") {
                std::cout << "Plant is ready for sale. Moving to sales floor inventory..." << std::endl;
                
                // Get the inventory manager (Singleton)
                InventoryManager* inventory = InventoryManager::getInstance();
                
                // Move the plant from greenhouse to sales floor
                if (inventory->isPlantInGreenhouse(plantReceiver)) {
                    inventory->removeFromGreenhouse(plantReceiver);
                    inventory->moveToSalesFloor(plantReceiver);
                    std::cout << "Plant successfully moved to sales floor inventory!" << std::endl;
                } else {
                    std::cout << "Warning: Plant was not found in greenhouse inventory." << std::endl;
                }
            } else {
                std::cout << "Plant is not ready for sale (Current state: " 
                         << plantReceiver->getCurrentStateName() 
                         << "). Cannot move to sales floor." << std::endl;
            }
        } else {
            std::cout << "Error: No plant receiver set for MoveToSalesFloorCommand." << std::endl;
        }
    }

    std::string getType() const override { return "MoveToSalesFloor"; }
    std::string getRequiredRole() const override { return "Sales"; }

    Command* clone() const override {
        MoveToSalesFloorCommand* cloned = new MoveToSalesFloorCommand();
        cloned->plantReceiver = this->plantReceiver;
        return cloned;
    }
};

#endif // MOVE_TO_SALES_FLOOR_COMMAND_H
