#include <iostream>
#include "GreenhouseSystem.h"
#include "GreenhouseDirector.h"
#include "PlantGroup.h"

int main() {
    // Create the Concrete Builder
    GreenhouseBuilder* greenhouseBuilder = new GreenhouseSystem();

    // Create PlantData for the Director
    std::vector<PlantData*> allPlants = {
        new PlantData("Rose_001", "Hybrid Tea, Red", "Shrub", "Summer", 15.50),
        new PlantData("Rose_002", "Floribunda, Pink", "Shrub", "Summer", 12.00),
        new PlantData("Pine_001", "Eastern White Pine", "Tree", "Winter", 45.99),
        new PlantData("Pine_002", "Scots Pine", "Tree", "Winter", 35.00)
    };
    // NOTE: For the Director to work, you must implement the construct()
    // method in GreenhouseDirector.cpp to iterate through the plants 
    // and call the appropriate builder methods.
    GreenhouseDirector director(greenhouseBuilder);
    director.setPlants(allPlants);

    // 4. Instruct the Director to Construct the Greenhouse
    // This call will reset the builder and call addCategory, addType, addSpecies, 
    // and finally addPlant for each PlantData object.
    GreenhouseComponent* greenhouse = director.construct();

    // 5. Print to verify structure (This should now include PlantPot leaves)
    std::cout << "=== FINAL GREENHOUSE STRUCTURE ===" << std::endl;
    // NOTE: Assuming your PlantPot::print() prints its ID/Info
    greenhouse->print();

    // Cleanup
    for (PlantData* plant : allPlants) {
        delete plant;
    }
// This should recursively delete the component hierarchy
    delete greenhouseBuilder;
    return 0;
}
