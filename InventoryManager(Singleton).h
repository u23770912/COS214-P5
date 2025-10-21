#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include <vector>
#include <string>

/** 
 * @file InventoryManager.h
 * @brief Inventory management system for plant nursery.
 * @details This file implements the sole usage of the Singleton design pattern to manage the inventory of pots and plants in a plant nursery.
 *          The InventoryManager class provides methods to add, remove, and retrieve pots and plants from the inventory.
 *          This class serves as a form of "database" for the nursery's inventory system.
 */

class Pot;
class Plant;

class InventoryManager {
    private:
        static InventoryManager* instance;
        std::vector<Pot> pots;

        // Different categories of plants (add more as needed or one general list can be used)
        std::vector<Plant> succulents;
        std::vector<Plant> roses;
        std::vector<Plant> tulips;
        std::vector<Plant> daisies;
    
        InventoryManager() {}

    public:
        static InventoryManager* getInstance() {
            if (!instance) {
                instance = new InventoryManager();
            }
            return instance;
        }

        void addPot(const Pot& pot  );

        void removePot(const Pot& pot);

        void addPlant(const Plant& plant);

        void removePlant(const Plant& plant);

        std::vector<Pot> getPots() const;

        std::vector<Plant> getPlants() const;
};

#endif