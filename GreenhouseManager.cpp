#include "GreenhouseManager.h"
#include <iostream>
#include <iomanip>

GreenhouseManager::GreenhouseManager() : builder(nullptr), root(nullptr) {
    builder = new GreenhouseSystem();
}

GreenhouseManager::~GreenhouseManager() {
    // Clean up plant pots (they don't own the PlantProduct due to aggregation)
    for (std::map<std::string, PlantPot*>::iterator it = plantPots.begin(); 
         it != plantPots.end(); ++it) {
        delete it->second;
    }
    plantPots.clear();
    
    // Builder owns the root, so deleting builder cleans up the tree
    delete builder;
}

GreenhouseComponent* GreenhouseManager::buildGreenhouseStructure() {
    // Reset and build default greenhouse structure
    builder->reset();
    
    // Build structure: Category -> Type -> Species
    // Category: Flowers
    builder->addCategory("Flowers")
           ->addType("Ornamental")
           ->addSpecies("Rose");
    speciesNodes["Rose"] = builder->getLastSpeciesNode();
    
    builder->addCategory("Flowers")
           ->addType("Ornamental")
           ->addSpecies("Orchid");
    speciesNodes["Orchid"] = builder->getLastSpeciesNode();
    
    builder->addCategory("Flowers")
           ->addType("Ornamental")
           ->addSpecies("Tulip");
    speciesNodes["Tulip"] = builder->getLastSpeciesNode();
    
    // Category: Trees
    builder->addCategory("Trees")
           ->addType("Bonsai")
           ->addSpecies("Bonsai");
    speciesNodes["Bonsai"] = builder->getLastSpeciesNode();
    
    builder->addCategory("Trees")
           ->addType("Sapling")
           ->addSpecies("Oak Sapling");
    speciesNodes["Oak"] = builder->getLastSpeciesNode();  // Map "Oak" to "Oak Sapling"
    
    builder->addCategory("Trees")
           ->addType("Sapling")
           ->addSpecies("Maple");
    speciesNodes["Maple"] = builder->getLastSpeciesNode();
    
    // Category: Succulents
    builder->addCategory("Succulents")
           ->addType("Desert")
           ->addSpecies("Aloe Vera");
    speciesNodes["Aloe"] = builder->getLastSpeciesNode();  // Map "Aloe" to "Aloe Vera"
    
    builder->addCategory("Succulents")
           ->addType("Desert")
           ->addSpecies("Echeveria");
    speciesNodes["Echeveria"] = builder->getLastSpeciesNode();
    
    builder->addCategory("Succulents")
           ->addType("Desert")
           ->addSpecies("Cactus");
    speciesNodes["Cactus"] = builder->getLastSpeciesNode();
    
    // Get the constructed greenhouse
    root = builder->getGreenhouse();
    
    return root;
}

bool GreenhouseManager::addPlantToStructure(PlantProduct* plant) {
    if (!plant || !root) {
        return false;
    }
    
    std::string speciesName = plant->getProfile()->getSpeciesName();
    
    // Try to find species node (from cache or by searching)
    PlantGroup* speciesNode = nullptr;
    
    if (speciesNodes.find(speciesName) != speciesNodes.end()) {
        speciesNode = speciesNodes[speciesName];
    } else {
        speciesNode = findSpeciesNode(speciesName);
        if (speciesNode) {
            speciesNodes[speciesName] = speciesNode;  // Cache it
        }
    }
    
    if (!speciesNode) {
        std::cerr << "Warning: Species '" << speciesName << "' not found in greenhouse structure!" << std::endl;
        return false;
    }
    
    // Create pot and assign plant to it (aggregation pattern)
    std::string potId = "POT-" + plant->getId();
    PlantPot* pot = new PlantPot(potId, "Greenhouse location for " + speciesName);
    pot->assignPlant(plant);  // Weak reference, doesn't own the plant
    
    // Add pot to species node
    speciesNode->add(pot);
    
    // Track the pot for later removal
    plantPots[plant->getId()] = pot;
    
    return true;
}

bool GreenhouseManager::removePlantFromStructure(const std::string& plantId) {
    if (plantPots.find(plantId) == plantPots.end()) {
        return false;  // Plant not in structure
    }
    
    PlantPot* pot = plantPots[plantId];
    
    // Remove plant reference from pot (aggregation - doesn't delete PlantProduct)
    pot->removePlant();
    
    // Note: Removing pot from parent requires parent reference
    // For now, we'll just delete the pot and remove from tracking
    // The parent PlantGroup should handle cleanup
    
    delete pot;
    plantPots.erase(plantId);
    
    return true;
}

void GreenhouseManager::displayGreenhouseStructure() {
    if (!root) {
        std::cout << "Greenhouse structure not initialized!" << std::endl;
        return;
    }
    
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║           🏛️  GREENHOUSE STRUCTURE VISUALIZATION 🏛️            ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
    
    root->display();
    
    std::cout << "\n";
    std::cout << "Total plants in structure: " << getTotalPlantCount() << "\n";
    std::cout << "════════════════════════════════════════════════════════════════\n";
}

void GreenhouseManager::displayPlantsByState(const std::string& state) {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║  Plants in State: " << std::setw(43) << std::left << state << "║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\n";
    
    int count = 0;
    for (std::map<std::string, PlantPot*>::const_iterator it = plantPots.begin();
         it != plantPots.end(); ++it) {
        PlantPot* pot = it->second;
        if (pot->hasPlant()) {
            PlantProduct* plant = pot->getPlant();
            if (plant && plant->getCurrentStateName() == state) {
                std::cout << "  ";
                pot->display();
                std::cout << "\n";
                count++;
            }
        }
    }
    
    if (count == 0) {
        std::cout << "  No plants in state '" << state << "'\n";
    } else {
        std::cout << "\n  Total: " << count << " plant(s)\n";
    }
    std::cout << "════════════════════════════════════════════════════════════════\n";
}

int GreenhouseManager::getTotalPlantCount() const {
    return plantPots.size();
}

void GreenhouseManager::registerSpeciesNode(const std::string& speciesName, PlantGroup* node) {
    speciesNodes[speciesName] = node;
}

PlantGroup* GreenhouseManager::findSpeciesNode(const std::string& speciesName) {
    // Species nodes are now cached in speciesNodes map during build
    // This function is only called if the species isn't in the cache
    // which means it doesn't exist in the structure
    return nullptr;
}
