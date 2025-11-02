#ifndef PLANTPOT_H
#define PLANTPOT_H

#include "GreenhouseComponent.h"
#include <string>

// Forward declaration to avoid circular dependency
class PlantProduct;

/**
 * @class PlantPot
 * @brief Leaf node in Composite pattern representing a plant location in greenhouse
 * 
 * PlantPot represents a physical location/slot in the greenhouse structure where
 * a PlantProduct can be placed. It uses AGGREGATION (weak reference) to associate
 * with a PlantProduct, meaning:
 * - PlantPot does NOT own the PlantProduct
 * - PlantProduct lifecycle is managed by InventoryManager
 * - When PlantProduct moves to sales floor, PlantPot remains empty but intact
 * - PlantPot can be reused for new plants
 * 
 * Design: Leaf Node in Composite Pattern with Aggregation
 */
class PlantPot : public GreenhouseComponent
{
private:
    std::string id;          // Pot/Location ID (e.g., "POT-001", "Slot-A1")
    std::string info;        // Additional info (location description, zone, etc.)
    PlantProduct* plantRef;  // Aggregation: weak reference to current plant (can be NULL)

public:
    /**
     * @brief Constructor for empty pot
     * @param id Pot identifier
     * @param info Additional information about this location
     */
    PlantPot(std::string id, std::string info);
    
    /**
     * @brief Destructor - does NOT delete the PlantProduct
     */
    ~PlantPot();
    
    // Getters
    std::string getId();
    std::string getInfo();
    PlantProduct* getPlant() const { return plantRef; }
    
    /**
     * @brief Check if pot currently has a plant
     */
    bool hasPlant() const { return plantRef != nullptr; }
    
    /**
     * @brief Assign a plant to this pot (aggregation - does not take ownership)
     * @param plant Pointer to PlantProduct (managed by InventoryManager)
     */
    void assignPlant(PlantProduct* plant);
    
    /**
     * @brief Remove plant reference from pot (plant itself remains in InventoryManager)
     */
    void removePlant();

    // GreenhouseComponent interface implementation
    void add(GreenhouseComponent* component) override; 
    void print() override;
    void display() override;
    void getPlantCount() override;
    void remove(GreenhouseComponent* item) override;
};

#endif