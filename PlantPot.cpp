#include "PlantPot.h"
#include "PlantProduct.h"
#include <iostream>
#include <iomanip>

PlantPot::PlantPot(std::string id, std::string info) 
    : id(id), info(info), plantRef(nullptr) {
    // Pot starts empty
}

PlantPot::~PlantPot() {
    // IMPORTANT: Do NOT delete plantRef
    // PlantProduct is owned by InventoryManager, not by PlantPot
    // This is aggregation, not composition
}

std::string PlantPot::getId() {
    return this->id;
}

std::string PlantPot::getInfo() {
    return this->info;
}

void PlantPot::assignPlant(PlantProduct* plant) {
    plantRef = plant;
}

void PlantPot::removePlant() {
    plantRef = nullptr;  // Just remove reference, don't delete
}

void PlantPot::add(GreenhouseComponent* component) {
    // Leaf nodes cannot have children
    std::cout << "Cannot add children to PlantPot (leaf node)" << std::endl;
}

void PlantPot::print(const std::string& prefix, bool isLast) {
    std::cout << prefix << (isLast ? "└── " : "├── ") 
              << "PlantPot ID: " << id << ", Info: " << info;
    
    if (plantRef) {
        std::cout << " [" << plantRef->getProfile()->getSpeciesName() 
                  << " - " << plantRef->getCurrentStateName() << "]";
    } else {
        std::cout << " [Empty]";
    }
    std::cout << std::endl;
}

void PlantPot::display() {
    std::cout << "  📍 Pot: " << getId();
    if (!info.empty()) {
        std::cout << " - " << info;
    }
    std::cout << std::endl;
    
    if (plantRef) {
        std::cout << "     🌱 Plant ID: " << plantRef->getId() << std::endl;
        std::cout << "        Species: " << plantRef->getProfile()->getSpeciesName() << std::endl;
        std::cout << "        State: " << plantRef->getCurrentStateName() << std::endl;
        std::cout << "        Time in State: " << plantRef->getSecondsInCurrentState() << "s" << std::endl;
        std::cout << "        Days in State: " << plantRef->getDaysInCurrentState() << " day(s)" << std::endl;
        
        // Display care requirements
        std::cout << "        Water Needs: " << plantRef->getProfile()->getProperty("idealWater") << std::endl;
        std::cout << "        Sunlight: " << plantRef->getProfile()->getProperty("idealSunlight") << std::endl;
        std::cout << "        Soil: " << plantRef->getProfile()->getProperty("idealSoil") << std::endl;
    } else {
        std::cout << "     [No plant assigned]" << std::endl;
    }
}

void PlantPot::getPlantCount() {
    if (plantRef) {
        std::cout << "1 plant";
    } else {
        std::cout << "0 plants (empty pot)";
    }
}

void PlantPot::remove(GreenhouseComponent* item) {
    // Leaf nodes have no children to remove
    std::cout << "Cannot remove children from PlantPot (leaf node)" << std::endl;
}
