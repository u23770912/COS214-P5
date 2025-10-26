#include <iostream>
#include <string>

// Sales Floor System Includes (Focus on working components)
#include "PlantProduct.h"
#include "MoveToSalesFloorCommand.h"
#include "InventoryManager.h"
#include "BridgeDP/FlowerProfile.h"

// Order System Includes (Commented out until implementations are ready)
// #include "Customer.h"
// #include "ConcreteOrderBuilder.h" 
// #include "PlaceOrderCommand.h"

void testOrderingSystem() {
    std::cout << "\n=== TESTING ORDER SYSTEM (PLACEHOLDER) ===" << std::endl;
    std::cout << "Order system testing is ready to be implemented once all dependencies are available." << std::endl;
    std::cout << "This will include Customer, ConcreteOrderBuilder, PlaceOrderCommand, etc." << std::endl;
    std::cout << "Order system test placeholder completed!\n" << std::endl;
}

void testSalesFloorSystem() {
    std::cout << "\n=== TESTING SALES FLOOR SYSTEM ===" << std::endl;
    
    // 1. Create multiple plant products with different profiles
    std::cout << "\n1. Creating plant products..." << std::endl;
    FlowerProfile* roseProfile = new FlowerProfile("Rose", "moderate", "full sun", "well-drained");
    FlowerProfile* tulipProfile = new FlowerProfile("Tulip", "moderate", "partial sun", "moist soil");
    
    PlantProduct* rose = new PlantProduct("ROSE001", roseProfile);
    PlantProduct* tulip = new PlantProduct("TULIP001", tulipProfile);
    
    // 2. Get inventory manager and add plants to greenhouse
    std::cout << "\n2. Adding plants to greenhouse inventory..." << std::endl;
    InventoryManager* inventory = InventoryManager::getInstance();
    inventory->addToGreenhouse(rose);
    inventory->addToGreenhouse(tulip);
    
    std::cout << "Initial greenhouse count: " << inventory->getGreenHouseInventory().size() << std::endl;
    std::cout << "Initial sales floor count: " << inventory->getReadyForSalePlants().size() << std::endl;
    
    // 3. Test MoveToSalesFloorCommand when plants are NOT ready
    std::cout << "\n3. Testing move command when plants are NOT ready for sale..." << std::endl;
    MoveToSalesFloorCommand* moveCommand1 = new MoveToSalesFloorCommand();
    moveCommand1->setReceiver(rose);
    std::cout << "Rose current state: " << rose->getCurrentStateName() << std::endl;
    moveCommand1->execute();
    
    // 4. Test command properties
    std::cout << "\n4. Testing MoveToSalesFloorCommand properties..." << std::endl;
    std::cout << "Command type: " << moveCommand1->getType() << std::endl;
    std::cout << "Required role: " << moveCommand1->getRequiredRole() << std::endl;
    
    // 5. Test command cloning
    std::cout << "\n5. Testing command cloning..." << std::endl;
    MoveToSalesFloorCommand* clonedCommand = dynamic_cast<MoveToSalesFloorCommand*>(moveCommand1->clone());
    if (clonedCommand) {
        std::cout << "Command cloned successfully!" << std::endl;
        std::cout << "Cloned command type: " << clonedCommand->getType() << std::endl;
        clonedCommand->setReceiver(tulip);
        clonedCommand->execute();
        delete clonedCommand;
    }
    
    // 6. Test multiple plants and inventory management
    std::cout << "\n6. Testing inventory management with multiple plants..." << std::endl;
    std::cout << "Greenhouse count: " << inventory->getGreenHouseInventory().size() << std::endl;
    std::cout << "Sales floor count: " << inventory->getReadyForSalePlants().size() << std::endl;
    
    // Test removing plants from greenhouse manually
    inventory->removeFromGreenhouse(tulip);
    inventory->moveToSalesFloor(tulip);
    
    std::cout << "After manual move - Greenhouse: " << inventory->getGreenHouseInventory().size() << std::endl;
    std::cout << "After manual move - Sales floor: " << inventory->getReadyForSalePlants().size() << std::endl;
    
    // 7. Test plant profile information
    std::cout << "\n7. Testing plant profile information..." << std::endl;
    std::cout << "Rose species: " << rose->getProfile()->getSpeciesName() << std::endl;
    std::cout << "Rose water needs: " << rose->getProfile()->getProperty("idealWater") << std::endl;
    std::cout << "Tulip species: " << tulip->getProfile()->getSpeciesName() << std::endl;
    std::cout << "Tulip sunlight needs: " << tulip->getProfile()->getProperty("idealSunlight") << std::endl;
    
    // Cleanup
    delete moveCommand1;
    delete rose;
    delete tulip;
    
    std::cout << "Sales floor system test completed!\n" << std::endl;
}

void testInventoryManager() {
    std::cout << "\n=== TESTING INVENTORY MANAGER ===" << std::endl;
    
    InventoryManager* inventory = InventoryManager::getInstance();
    
    // Create test plants
    FlowerProfile* profile1 = new FlowerProfile("Daisy", "low", "full sun", "sandy");
    FlowerProfile* profile2 = new FlowerProfile("Sunflower", "high", "full sun", "loamy");
    
    PlantProduct* daisy = new PlantProduct("DAISY001", profile1);
    PlantProduct* sunflower = new PlantProduct("SUNFLOWER001", profile2);
    
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
    
    std::cout << "Inventory manager test completed!\n" << std::endl;
}

int main() {
    std::cout << "=== COMPREHENSIVE SALES FLOOR TESTING ===" << std::endl;
    
    try {
        // Test all available systems
        testInventoryManager();
        testSalesFloorSystem();
        testOrderingSystem();  // Placeholder for now
        
        std::cout << "\n=== ALL TESTS COMPLETED SUCCESSFULLY ===" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "Exception caught during testing: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "Unknown exception caught during testing!" << std::endl;
        return 1;
    }
    
    return 0;
}


