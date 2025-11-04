#include <iostream>
#include "GreenhouseSystem.h"
#include "GreenhouseDirector.h"
#include "PlantGroup.h"
#include "PlantPot.h" // NEW: Required to define PlantPot for the removal test

// and is not required for the isolated removal test.

int main() {
    // === 1. Original Director Construction Test ===
    std::cout << "--- 1. Testing Director Construction ---" << std::endl;
    GreenhouseBuilder* greenhouseBuilder = new GreenhouseSystem();

    // The plants are designed to be sorted by Category, Type, Species for Director to work.
    std::vector<PlantData*> allPlants = {
        new PlantData("Rose_001", "Hybrid Tea, Red", "Shrub", "Summer", 15.50),
        new PlantData("Rose_002", "Floribunda, Pink", "Shrub", "Summer", 12.00),
        new PlantData("Pine_001", "Eastern White Pine", "Tree", "Winter", 45.99),
        new PlantData("Pine_002", "Scots Pine", "Tree", "Winter", 35.00)
    };
    GreenhouseDirector director(greenhouseBuilder);
    director.setPlants(allPlants);
    // Instruct the Director to Construct the Greenhouse
    GreenhouseComponent* greenhouse = director.construct();
    std::cout << "=== GREENHOUSE STRUCTURE BUILT BY DIRECTOR ===" << std::endl;
    greenhouse->print();
    std::cout << "-----------------------------------------------" << std::endl;
    // Cleanup PlantData objects (as they are independent of the GreenhouseComponent hierarchy)
    for (PlantData* plant : allPlants) {
        delete plant;
    }
    allPlants.clear(); // Clear the vector after deletion

    // Cleanup the initial greenhouse structure via builder reset
    greenhouseBuilder->reset(); 
    greenhouse = nullptr; // Set to nullptr as reset frees the memory
    
    // Testing Empty Greenhouse, Add, and Remove Operations ===
    std::cout << "\n--- 2. Testing Empty Greenhouse, Add, and Remove Operations ---" << std::endl;
    
    // Creating an Empty Greenhouse
    // The reset() call already creates the empty 'Greenhouse' root.
    greenhouse = greenhouseBuilder->getGreenhouse();
    std::cout << "=== EMPTY GREENHOUSE STRUCTURE ===" << std::endl;
    greenhouse->print();
    std::cout << "----------------------------------" << std::endl;
    
    // Adding a New Plant Group and a Plant (Manual Builder Use)
    std::cout << "=== Adding 'Indoor' Category, 'Succulent' Type, 'Haworthia' Species, and 'Hawa_001' Plant ===" << std::endl;
    PlantData* newPlantData = new PlantData("Hawa_001", "Zebra Plant", "Succulent", "Indoor", 7.99);

    // Manually build the structure for the new plant
    greenhouseBuilder
        ->addCategory(newPlantData->getCategory()) 
        ->addType(newPlantData->getType())
        ->addSpecies(newPlantData->getSpecies())
        ->addPlant(newPlantData);
    
    std::cout << "=== GREENHOUSE STRUCTURE AFTER ADDING Hawa_001 ===" << std::endl;
    greenhouse->print();
    std::cout << "------------------------------------------------" << std::endl;

    // Demonstrating PlantGroup Removal Logic
    // This section isolates the test for PlantGroup::remove
    std::cout << "\n=== . DEMONSTRATING PLANTGROUP REMOVAL LOGIC ===" << std::endl;

    // Create a temporary parent group (PlantGroup)
    GreenhouseComponent* tempGroup = new PlantGroup("Test Removable Group");

    // Create the leaf nodes (PlantPot) and define the necessary pointers.
    PlantPot* plantToRemove = new PlantPot("TEST_001", "To be removed");
    PlantPot* plantToKeep = new PlantPot("TEST_002", "To be kept");

    // Add both plants to the group
    tempGroup->add(plantToRemove);
    tempGroup->add(plantToKeep);

    std::cout << "--- Test Group Before Removal ---" << std::endl;
    tempGroup->print();

    //  Perform the Removal
    std::cout << "\nAttempting to remove TEST_001 from Test Removable Group..." << std::endl;
    tempGroup->remove(plantToRemove); 

    std::cout << "--- Test Group After Removal ---" << std::endl;
    tempGroup->print();

    // Cleanup for the temporary test structure
    // 'plantToRemove' was deleted inside PlantGroup::remove.
    // Deleting tempGroup will correctly clean up 'plantToKeep' via the PlantGroup destructor.
    delete tempGroup;
    
    // Cleanup of the manually created PlantData from the ADD test
    delete newPlantData;
    
    // Cleanup the final greenhouse structure and builder
    // This will recursively delete the root and all children.
    delete greenhouseBuilder;
    
    return 0;
}