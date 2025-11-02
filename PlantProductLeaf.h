#ifndef PLANTPRODUCTLEAF_H
#define PLANTPRODUCTLEAF_H

#include "GreenhouseComponent.h"
#include "PlantProduct.h"
#include <iostream>
#include <iomanip>

/**
 * @class PlantProductLeaf
 * @brief Leaf node in Composite pattern that represents individual PlantProduct instances
 * 
 * This class serves as an adapter/wrapper that allows PlantProduct to participate
 * in the GreenhouseComponent tree structure through aggregation (weak association).
 * 
 * Key Design Decisions:
 * - PlantProduct is stored by POINTER (aggregation, not composition)
 * - The PlantProductLeaf does NOT own the PlantProduct
 * - When PlantProduct moves to sales floor or withers, this leaf can be removed
 *   from the tree without deleting the PlantProduct
 * - The GreenhouseComponent structure (Category/Type/Species) remains intact
 *   and can be reused for new plants
 * 
 * Lifecycle:
 * 1. PlantProduct created with species profile
 * 2. PlantProductLeaf created referencing the PlantProduct
 * 3. Leaf added to appropriate Species node in greenhouse structure
 * 4. When PlantProduct reaches ReadyForSale or Withering:
 *    - Leaf removed from tree (InventoryManager owns PlantProduct)
 *    - GreenhouseComponent structure unchanged and reusable
 */
class PlantProductLeaf : public GreenhouseComponent {
private:
    PlantProduct* plantRef;  // Aggregation: weak reference, does NOT own

public:
    /**
     * @brief Constructor - creates leaf referencing a PlantProduct
     * @param plant Pointer to PlantProduct (NOT owned by this class)
     */
    PlantProductLeaf(PlantProduct* plant);
    
    /**
     * @brief Destructor - does NOT delete the PlantProduct
     */
    ~PlantProductLeaf();
    
    /**
     * @brief Get the referenced PlantProduct
     * @return Pointer to PlantProduct
     */
    PlantProduct* getPlant() const { return plantRef; }
    
    // GreenhouseComponent interface implementation
    void print() override;
    void display() override;
    void add(GreenhouseComponent* item) override;
    void remove(GreenhouseComponent* item) override;
    void getPlantCount() override;
};

#endif // PLANTPRODUCTLEAF_H
