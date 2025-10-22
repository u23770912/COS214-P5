#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include "LifecycleObserver.h"
#include "PlantProduct.h"

// Singleton and Concrete Observer
class InventoryManager : public LifecycleObserver {
private:
    InventoryManager();
    static InventoryManager* instance;

    int plantsInStock;

public:
    InventoryManager(const InventoryManager&) = delete;
    void operator=(const InventoryManager&) = delete;

    static InventoryManager* getInstance();

    // From LifecycleObserver
    void update(PlantProduct* plant) override;

    int getStockCount() const;
};

#endif // INVENTORY_MANAGER_H
