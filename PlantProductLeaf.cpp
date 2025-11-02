#include "PlantProductLeaf.h"

PlantProductLeaf::PlantProductLeaf(PlantProduct* plant) : plantRef(plant) {
    // Aggregation: we reference the plant but don't own it
}

PlantProductLeaf::~PlantProductLeaf() {
    // IMPORTANT: Do NOT delete plantRef
    // The PlantProduct is owned by InventoryManager
    // This leaf is just a temporary reference in the greenhouse structure
}

void PlantProductLeaf::print() {
    if (!plantRef) {
        std::cout << "[NULL PLANT]";
        return;
    }
    
    // Display compact plant information
    std::cout << plantRef->getId() 
              << " (" << plantRef->getCurrentStateName() 
              << ", " << plantRef->getSecondsInCurrentState() << "s)";
}

void PlantProductLeaf::display() {
    if (!plantRef) {
        std::cout << "NULL Plant Reference" << std::endl;
        return;
    }
    
    // Display detailed plant information with formatting
    std::cout << "  🌱 Plant ID: " << plantRef->getId() << std::endl;
    std::cout << "     Species: " << plantRef->getProfile()->getSpeciesName() << std::endl;
    std::cout << "     State: " << plantRef->getCurrentStateName() << std::endl;
    std::cout << "     Time in State: " << plantRef->getSecondsInCurrentState() << "s" << std::endl;
    std::cout << "     Days in State: " << plantRef->getDaysInCurrentState() << " day(s)" << std::endl;
    
    // Display care requirements
    std::cout << "     Water Needs: " << plantRef->getProfile()->getProperty("idealWater") << std::endl;
    std::cout << "     Sunlight: " << plantRef->getProfile()->getProperty("idealSunlight") << std::endl;
    std::cout << "     Soil: " << plantRef->getProfile()->getProperty("idealSoil") << std::endl;
}

void PlantProductLeaf::add(GreenhouseComponent* item) {
    // Leaf nodes cannot have children
    std::cout << "Cannot add child to PlantProductLeaf (leaf node)" << std::endl;
}

void PlantProductLeaf::remove(GreenhouseComponent* item) {
    // Leaf nodes have no children to remove
    std::cout << "Cannot remove child from PlantProductLeaf (leaf node)" << std::endl;
}

void PlantProductLeaf::getPlantCount() {
    // A leaf represents exactly 1 plant
    std::cout << "1 plant";
}
