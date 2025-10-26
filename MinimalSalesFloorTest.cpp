#include <iostream>
#include <string>

// Minimal Sales Floor Testing - Core Functionality Only
#include "MoveToSalesFloorCommand.h"
#include "InventoryManager.h"
#include "PlantProduct.h"
#include "BridgeDP/FlowerProfile.h"

// Simple test function for move to sales floor functionality
void testMoveToSalesFloorCore() {
    std::cout << "\n=== TESTING MOVE TO SALES FLOOR - CORE FUNCTIONALITY ===" << std::endl;
    
    try {
        // 1. Create a plant product
        std::cout << "\n1. Creating plant product..." << std::endl;
        FlowerProfile* roseProfile = new FlowerProfile("Rose", "moderate", "full sun", "well-drained");
        PlantProduct* rose = new PlantProduct("ROSE001", roseProfile);
        
        // 2. Test inventory manager
        std::cout << "\n2. Testing inventory manager..." << std::endl;
        InventoryManager* inventory = InventoryManager::getInstance();
        
        std::cout << "Initial greenhouse count: " << inventory->getGreenHouseInventory().size() << std::endl;
        std::cout << "Initial sales floor count: " << inventory->getReadyForSalePlants().size() << std::endl;
        
        // 3. Add plant to greenhouse
        inventory->addToGreenhouse(rose);
        std::cout << "After adding to greenhouse: " << inventory->getGreenHouseInventory().size() << std::endl;
        std::cout << "Plant in greenhouse: " << (inventory->isPlantInGreenhouse(rose) ? "Yes" : "No") << std::endl;
        
        // 4. Test MoveToSalesFloorCommand creation
        std::cout << "\n3. Testing MoveToSalesFloorCommand..." << std::endl;
        MoveToSalesFloorCommand* moveCommand = new MoveToSalesFloorCommand();
        moveCommand->setReceiver(rose);
        
        std::cout << "Command type: " << moveCommand->getType() << std::endl;
        std::cout << "Required role: " << moveCommand->getRequiredRole() << std::endl;
        
        // 5. Execute command (should fail because plant is not in ReadyForSale state)
        std::cout << "\n4. Executing move command..." << std::endl;
        std::cout << "Plant current state: " << rose->getCurrentStateName() << std::endl;
        moveCommand->execute();
        
        // 6. Check inventory after execution
        std::cout << "\nAfter move command execution:" << std::endl;
        std::cout << "Greenhouse count: " << inventory->getGreenHouseInventory().size() << std::endl;
        std::cout << "Sales floor count: " << inventory->getReadyForSalePlants().size() << std::endl;
        
        // 7. Test command cloning
        std::cout << "\n5. Testing command cloning..." << std::endl;
        MoveToSalesFloorCommand* clonedCommand = dynamic_cast<MoveToSalesFloorCommand*>(moveCommand->clone());
        if (clonedCommand) {
            std::cout << "Command cloned successfully!" << std::endl;
            std::cout << "Cloned command type: " << clonedCommand->getType() << std::endl;
            delete clonedCommand;
        }
        
        // Cleanup
        delete moveCommand;
        delete rose;
        
        std::cout << "\n=== CORE FUNCTIONALITY TEST COMPLETED ===" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unknown exception caught!" << std::endl;
    }
}

void testInventoryManagerStandalone() {
    std::cout << "\n=== TESTING INVENTORY MANAGER STANDALONE ===" << std::endl;
    
    // Create test plants
    FlowerProfile* profile1 = new FlowerProfile("Daisy", "low", "full sun", "sandy");
    FlowerProfile* profile2 = new FlowerProfile("Sunflower", "high", "full sun", "loamy");
    
    PlantProduct* daisy = new PlantProduct("DAISY001", profile1);
    PlantProduct* sunflower = new PlantProduct("SUNFLOWER001", profile2);
    
    InventoryManager* inventory = InventoryManager::getInstance();
    
    // Test adding to greenhouse
    std::cout << "\nAdding plants to greenhouse..." << std::endl;
    inventory->addToGreenhouse(daisy);
    inventory->addToGreenhouse(sunflower);
    
    // Test checking if plant is in greenhouse
    std::cout << "Daisy in greenhouse: " << (inventory->isPlantInGreenhouse(daisy) ? "Yes" : "No") << std::endl;
    std::cout << "Sunflower in greenhouse: " << (inventory->isPlantInGreenhouse(sunflower) ? "Yes" : "No") << std::endl;
    
    // Test moving to sales floor
    std::cout << "\nMoving daisy to sales floor..." << std::endl;
    inventory->removeFromGreenhouse(daisy);
    inventory->moveToSalesFloor(daisy);
    
    std::cout << "Stock count (ready for sale): " << inventory->getStockCount() << std::endl;
    std::cout << "Daisy in greenhouse: " << (inventory->isPlantInGreenhouse(daisy) ? "Yes" : "No") << std::endl;
    
    // Cleanup
    delete daisy;
    delete sunflower;
    
    std::cout << "Inventory manager standalone test completed!\n" << std::endl;
}

int main() {
    std::cout << "=== MINIMAL SALES FLOOR TESTING ===" << std::endl;
    
    testInventoryManagerStandalone();
    testMoveToSalesFloorCore();
    
    std::cout << "\n=== ALL MINIMAL TESTS COMPLETED ===" << std::endl;
    return 0;
}