#include "InventoryManager.h"
#include "PotDecorator/PotDecorator.h"
#include "PlantProduct.h"
#include <algorithm>
#include <iostream>

// Private constructor
InventoryManager::InventoryManager() : plantsInStock(0) {
    std::cout << "InventoryManager database initialized." << std::endl;
}

// Private destructor - automatic cleanup
InventoryManager::~InventoryManager() {
    std::cout << "InventoryManager database shutting down." << std::endl;
    cleanup();
}

// Manual cleanup method - call before program exit
void InventoryManager::cleanup() {
    std::cout << "Cleaning up InventoryManager resources..." << std::endl;
    
    // Clean up greenhouse plants
    for (PlantProduct* plant : greenHouseInventory) {
        delete plant;
    }
    greenHouseInventory.clear();
    
    // Clean up plants ready for sale
    for (PlantProduct* plant : readyForSalePlants) {
        delete plant;
    }
    readyForSalePlants.clear();
    
    // Clean up sold plants
    for (PlantProduct* plant : soldPlants) {
        delete plant;
    }
    soldPlants.clear();
    
    // Clean up pots
    // for (Pots* pot : potInventory) {
    //     delete pot;
    // }
    // potInventory.clear();
    
    plantsInStock = 0;
    std::cout << "InventoryManager cleanup complete." << std::endl;
}

// Meyer's Singleton - Thread-safe, automatic lifetime management
InventoryManager& InventoryManager::getInstance() {
    static InventoryManager instance; // Created once, destroyed automatically at program end
    return instance;  // Return reference, never null
}

void InventoryManager::update(PlantProduct* plant, const std::string& commandType) {
    std::cout << "InventoryManager received update for plant with command: " << commandType << std::endl;
    // Handle lifecycle updates as needed
}

int InventoryManager::getStockCount() const {
    return readyForSalePlants.size();
}

std::vector<PlantProduct*> InventoryManager::getGreenHouseInventory() const {
    return greenHouseInventory;
}

std::vector<PlantProduct*> InventoryManager::getReadyForSalePlants() const {
    return readyForSalePlants;
}

std::vector<PlantProduct*> InventoryManager::getSoldPlants() const {
    return soldPlants;
}

std::vector<Pots*> InventoryManager::getPotInventory() const {
    return potInventory;
}

void InventoryManager::addPot(Pots* pot) {
    if (pot) {
        potInventory.push_back(pot);
    }
}

// void InventoryManager::removePot(Pots* pot) {
//     auto it = std::find(potInventory.begin(), potInventory.end(), pot);
//     if (it != potInventory.end()) {
//         potInventory.erase(it);
//     }
// }

void InventoryManager::moveToSalesFloor(PlantProduct* plant) {
    if (plant) {
        // Check if plant is not already in sales floor
        auto it = std::find(readyForSalePlants.begin(), readyForSalePlants.end(), plant);
        if (it == readyForSalePlants.end()) {
            readyForSalePlants.push_back(plant);
            plantsInStock++;
            std::cout << "Plant moved to sales floor inventory. Total plants ready for sale: " 
                     << readyForSalePlants.size() << std::endl;
        } else {
            std::cout << "Plant is already in sales floor inventory." << std::endl;
        }
    }
}

void InventoryManager::addToGreenhouse(PlantProduct* plant) {
    if (plant) {
        // Check if plant is not already in greenhouse
        auto it = std::find(greenHouseInventory.begin(), greenHouseInventory.end(), plant);
        if (it == greenHouseInventory.end()) {
            greenHouseInventory.push_back(plant);
            std::cout << "Plant added to greenhouse inventory. Total plants in greenhouse: " 
                     << greenHouseInventory.size() << std::endl;
        } else {
            std::cout << "Plant is already in greenhouse inventory." << std::endl;
        }
    }
}

void InventoryManager::removeFromGreenhouse(PlantProduct* plant) {
    auto it = std::find(greenHouseInventory.begin(), greenHouseInventory.end(), plant);
    if (it != greenHouseInventory.end()) {
        greenHouseInventory.erase(it);
        std::cout << "Plant removed from greenhouse inventory. Remaining plants in greenhouse: " 
                 << greenHouseInventory.size() << std::endl;
    } else {
        std::cout << "Plant not found in greenhouse inventory." << std::endl;
    }
}

bool InventoryManager::isPlantInGreenhouse(PlantProduct* plant) const {
    auto it = std::find(greenHouseInventory.begin(), greenHouseInventory.end(), plant);
    return it != greenHouseInventory.end();
}

// Order validation methods
bool InventoryManager::isPlantAvailableForSale(const std::string& plantType, int quantity) const {
    return getAvailablePlantCount(plantType) >= quantity;
}

bool InventoryManager::isPotAvailable(const std::string& potType, int quantity) const {
    return getAvailablePotCount(potType) >= quantity;
}

std::vector<PlantProduct*> InventoryManager::getAvailablePlantsByType(const std::string& plantType) const {
    std::vector<PlantProduct*> availablePlants;
    
    for (PlantProduct* plant : readyForSalePlants) {
        if (plant && plant->getProfile() && 
            plant->getProfile()->getSpeciesName() == plantType) {
            availablePlants.push_back(plant);
        }
    }
    
    return availablePlants;
}

int InventoryManager::getAvailablePlantCount(const std::string& plantType) const {
    int count = 0;
    
    for (PlantProduct* plant : readyForSalePlants) {
        if (plant && plant->getProfile() && 
            plant->getProfile()->getSpeciesName() == plantType) {
            count++;
        }
    }
    
    return count;
}

int InventoryManager::getAvailablePotCount(const std::string& potType) const {
    int count = 0;
    
    // Note: This is a simplified implementation
    // In a real system, Pots would have a type/name property
    for (Pots* pot : potInventory) {
        if (pot) {
            // For now, assume all pots are available
            // In real implementation, check pot->getType() == potType
            count++;
        }
    }
    
    return count;
}

bool InventoryManager::reservePlantsForOrder(const std::string& plantType, int quantity) {
    std::vector<PlantProduct*> availablePlants = getAvailablePlantsByType(plantType);
    
    if (static_cast<int>(availablePlants.size()) >= quantity) {
        std::cout << "Reserved " << quantity << " " << plantType << " plants for order." << std::endl;
        return true;
    } else {
        std::cout << "Cannot reserve " << quantity << " " << plantType 
                  << " plants. Only " << availablePlants.size() << " available." << std::endl;
        return false;
    }
}

bool InventoryManager::reservePotsForOrder(const std::string& potType, int quantity) {
    int availablePots = getAvailablePotCount(potType);
    
    if (availablePots >= quantity) {
        std::cout << "Reserved " << quantity << " " << potType << " pots for order." << std::endl;
        return true;
    } else {
        std::cout << "Cannot reserve " << quantity << " " << potType 
                  << " pots. Only " << availablePots << " available." << std::endl;
        return false;
    }
}

void InventoryManager::releasePlantsFromOrder(const std::string& plantType, int quantity) {
    std::cout << "Released " << quantity << " " << plantType << " plants from order reservation." << std::endl;
}

void InventoryManager::releasePotsFromOrder(const std::string& potType, int quantity) {
    std::cout << "Released " << quantity << " " << potType << " pots from order reservation." << std::endl;
}

void InventoryManager::printInventoryReport() const {
    std::cout << "\n=== INVENTORY DATABASE REPORT ===" << std::endl;
    std::cout << "Greenhouse Inventory: " << greenHouseInventory.size() << " plants" << std::endl;
    std::cout << "Sales Floor Inventory: " << readyForSalePlants.size() << " plants" << std::endl;
    std::cout << "Sold Plants: " << soldPlants.size() << " plants" << std::endl;
    std::cout << "Pot Inventory: " << potInventory.size() << " pots" << std::endl;
    
    // Group plants by type
    std::cout << "\nPlants Ready for Sale by Type:" << std::endl;
    for (PlantProduct* plant : readyForSalePlants) {
        if (plant && plant->getProfile()) {
            std::cout << "  - " << plant->getProfile()->getSpeciesName() << std::endl;
        }
    }
    std::cout << "=================================" << std::endl;
}

bool InventoryManager::sellPlants(const std::string& plantType, int quantity) {
    std::vector<PlantProduct*> plantsToSell;
    
    // Find the required quantity of plants
    for (PlantProduct* plant : readyForSalePlants) {
        if (plant && plant->getProfile() && 
            plant->getProfile()->getSpeciesName() == plantType) {
            plantsToSell.push_back(plant);
            if ((int)plantsToSell.size() >= quantity) {
                break;
            }
        }
    }
    
    // Check if we have enough
    if ((int)plantsToSell.size() < quantity) {
        std::cout << "Cannot sell " << quantity << " " << plantType 
                  << " - only " << plantsToSell.size() << " available" << std::endl;
        return false;
    }
    
    // Move plants from sales floor to sold
    for (PlantProduct* plant : plantsToSell) {
        removeFromSalesFloor(plant);
        markAsSold(plant);
    }
    
    std::cout << "Successfully sold " << quantity << " " << plantType << " plant(s)" << std::endl;
    return true;
}

void InventoryManager::removeFromSalesFloor(PlantProduct* plant) {
    auto it = std::find(readyForSalePlants.begin(), readyForSalePlants.end(), plant);
    if (it != readyForSalePlants.end()) {
        readyForSalePlants.erase(it);
        plantsInStock--;
        std::cout << "  [Removed from sales floor: " 
                  << plant->getProfile()->getSpeciesName() << "]" << std::endl;
    }
}

void InventoryManager::markAsSold(PlantProduct* plant) {
    if (plant) {
        // Check if not already in sold list
        auto it = std::find(soldPlants.begin(), soldPlants.end(), plant);
        if (it == soldPlants.end()) {
            soldPlants.push_back(plant);
            std::cout << "  [Marked as sold: " 
                      << plant->getProfile()->getSpeciesName() << "]" << std::endl;
        }
    }
}



void InventoryManager::addCustomPot(Pot* pot) {
    if (pot) {
        potInventory.push_back(pot);
        std::cout << "[Inventory] Added pot: ";
        pot->print();
        std::cout << std::endl;
    }
}

Pot* InventoryManager::getPotByIndex(int index) {
    if (index >= 0 && index < (int)potInventory.size()) {
        return potInventory[index];
    }
    return nullptr;
}

void InventoryManager::displayPotInventory() const {
    std::cout << "\n=== POT INVENTORY ===" << std::endl;
    std::cout << "Total: " << potInventory.size() << " pots" << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    
    for (size_t i = 0; i < potInventory.size(); i++) {
        std::cout << (i+1) << ". ";
        potInventory[i]->print();
        
        PotDecorator* decorator = dynamic_cast<PotDecorator*>(potInventory[i]);
        if (decorator) {
            std::cout << " - R" << decorator->getPrice();
        } else {
            std::cout << " - R10.00";
        }
        std::cout << std::endl;
    }
    std::cout << std::string(70, '-') << std::endl;
}

double InventoryManager::getTotalPotInventoryValue() const {
    double total = 0.0;
    for (Pot* pot : potInventory) {
        PotDecorator* decorator = dynamic_cast<PotDecorator*>(pot);
        if (decorator) {
            total += decorator->getPrice();
        } else {
            total += 10.0;
        }
    }
    return total;
}

int InventoryManager::getPotInventoryCount() const {
    return potInventory.size();
}