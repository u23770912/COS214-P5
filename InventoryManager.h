#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include "LifecycleObserver.h"
#include "PlantProduct.h"
#include <vector>

class Pots;

// Singleton and Concrete Observer
class InventoryManager : public LifecycleObserver {
    
    private:
        InventoryManager();
        ~InventoryManager();
        static InventoryManager* instance;
        
        std::vector<PlantProduct*> greenHouseInventory;
        std::vector<PlantProduct*> readyForSalePlants;
        std::vector<Pots*> potInventory;

        int plantsInStock;

    protected: 
        InventoryManager(const InventoryManager&) = delete;
        InventoryManager& operator=(const InventoryManager&) = delete;

    public:
       
        static InventoryManager* getInstance() {
            if (!instance) {
                instance = new InventoryManager();
            }
            return instance;
        }

        static InventoryManager* getInstance();

        // From LifecycleObserver
        void update(PlantProduct* plant, const std::string& commandType) override;

        int getStockCount() const;

        std::vector<PlantProduct*> getGreenHouseInventory() const;
        std::vector<PlantProduct*> getReadyForSalePlants() const;
        std::vector<Pots*> getPotInventory() const;

        void addPot(Pots* pot);
        void removePot(Pots* pot);
        
};

#endif // INVENTORY_MANAGER_H
