#include "InventoryManager.h"
#include "PlantProduct.h"
#include <algorithm>
#include <iostream>

// Initialize static member
InventoryManager* InventoryManager::instance = nullptr;

InventoryManager::InventoryManager() : plantsInStock(0) {
    std::cout << "InventoryManager initialized." << std::endl;
}

InventoryManager::~InventoryManager() {
    // Clean up inventory vectors if needed
    // Note: We don't delete PlantProduct* as they may be owned elsewhere
}

InventoryManager* InventoryManager::getInstance() {
    if (!instance) {
        instance = new InventoryManager();
    }
    return instance;
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

std::vector<Pots*> InventoryManager::getPotInventory() const {
    return potInventory;
}

void InventoryManager::addPot(Pots* pot) {
    if (pot) {
        potInventory.push_back(pot);
    }
}

void InventoryManager::removePot(Pots* pot) {
    auto it = std::find(potInventory.begin(), potInventory.end(), pot);
    if (it != potInventory.end()) {
        potInventory.erase(it);
    }
}

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