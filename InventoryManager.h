#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include "LifecycleObserver.h"
#include "PlantProduct.h"
#include <vector>

class Pots;

/**
 * @brief Singleton Database-like Inventory Manager with Reference-Only Access
 * 
 * This design ensures:
 * - Only one instance exists (Singleton pattern)
 * - No heap allocation concerns (static storage)
 * - No dangling pointer issues (reference-only access)
 * - Clear ownership semantics (automatic lifetime management)
 * - Thread-safe initialization (C++11 guarantees)
 */
class InventoryManager : public LifeCycleObserver {
    
    private:
        // Private constructor - can't be instantiated externally
        InventoryManager();
        
        // Private destructor - automatic cleanup
        ~InventoryManager();
        
        // Database storage vectors
        std::vector<PlantProduct*> greenHouseInventory;
        std::vector<PlantProduct*> readyForSalePlants;
        std::vector<PlantProduct*> soldPlants;  // Plants that have been sold
        std::vector<Pots*> potInventory;

        int plantsInStock;

    public: 
        // Delete copy operations to maintain singleton property
        InventoryManager(const InventoryManager&) = delete;
        InventoryManager& operator=(const InventoryManager&) = delete;
        
        // Delete move operations for safety
        InventoryManager(InventoryManager&&) = delete;
        InventoryManager& operator=(InventoryManager&&) = delete;

        /**
         * @brief Get reference to the single InventoryManager instance
         * @return Reference to the singleton instance (never null, automatic lifetime)
         * 
         * Uses Meyer's Singleton pattern - guaranteed thread-safe initialization
         * and automatic destruction at program end
         */
        static InventoryManager& getInstance();

        // From LifecycleObserver
        void update(PlantProduct* plant, const std::string& commandType) override;

        int getStockCount() const;

        std::vector<PlantProduct*> getGreenHouseInventory() const;
        std::vector<PlantProduct*> getReadyForSalePlants() const;
        std::vector<PlantProduct*> getSoldPlants() const;

        // Manual cleanup method - call before program exit to avoid static destruction issues
        void cleanup();
        std::vector<Pots*> getPotInventory() const;

        void addPot(Pots* pot);
        void removePot(Pots* pot);
        
        // Methods for moving plants between greenhouse and sales floor
        void moveToSalesFloor(PlantProduct* plant);
        void addToGreenhouse(PlantProduct* plant);
        void removeFromGreenhouse(PlantProduct* plant);
        bool isPlantInGreenhouse(PlantProduct* plant) const;
        
        // Order validation methods for customer orders
        bool isPlantAvailableForSale(const std::string& plantType, int quantity) const;
        bool isPotAvailable(const std::string& potType, int quantity) const;
        std::vector<PlantProduct*> getAvailablePlantsByType(const std::string& plantType) const;
        
        // Reserve/release methods for order processing
        bool reservePlantsForOrder(const std::string& plantType, int quantity);
        bool reservePotsForOrder(const std::string& potType, int quantity);
        void releasePlantsFromOrder(const std::string& plantType, int quantity);
        void releasePotsFromOrder(const std::string& potType, int quantity);
        
        // Inventory search and reporting
        int getAvailablePlantCount(const std::string& plantType) const;
        int getAvailablePotCount(const std::string& potType) const;
        void printInventoryReport() const;
        
        // Methods for handling sold plants
        bool sellPlants(const std::string& plantType, int quantity);
        void removeFromSalesFloor(PlantProduct* plant);
        void markAsSold(PlantProduct* plant);
        
};

#endif // INVENTORY_MANAGER_H
