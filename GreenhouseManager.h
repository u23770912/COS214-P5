#ifndef GREENHOUSEMANAGER_H
#define GREENHOUSEMANAGER_H

#include "GreenhouseSystem.h"
#include "PlantPot.h"
#include "PlantProduct.h"
#include "PlantGroup.h"
#include <map>
#include <vector>
#include <string>

/**
 * @class GreenhouseManager
 * @brief Facade for managing greenhouse structure and plant assignments
 * 
 * This class extends GreenhouseSystem functionality by providing:
 * - Easy plant addition to correct species nodes
 * - Navigation through greenhouse hierarchy
 * - Visualization of entire greenhouse structure with all plants
 * - Plant removal when moving to sales floor
 * - Statistics and reporting
 * 
 * Design Pattern: Facade + Composite Management
 */
class GreenhouseManager {
private:
    GreenhouseSystem* builder;
    GreenhouseComponent* root;
    
    // Cache for quick lookup: maps species name to its GreenhouseComponent node
    std::map<std::string, PlantGroup*> speciesNodes;
    
    // Track all plant pots for easy removal
    std::map<std::string, PlantPot*> plantPots;  // plantID -> pot
    std::map<std::string, PlantGroup*> plantPotParents;  // plantID -> parent group
    
public:
    GreenhouseManager();
    ~GreenhouseManager();
    
    /**
     * @brief Initialize greenhouse structure using builder pattern
     * @return The root GreenhouseComponent
     */
    GreenhouseComponent* buildGreenhouseStructure();
    
    /**
     * @brief Add a PlantProduct to the greenhouse structure
     * @param plant The plant to add
     * @return true if successfully added, false if species not found in structure
     */
    bool addPlantToStructure(PlantProduct* plant);
    
    /**
     * @brief Remove a plant from the greenhouse structure
     * @param plantId The ID of the plant to remove
     * @return true if successfully removed
     */
    bool removePlantFromStructure(const std::string& plantId);
    
    /**
     * @brief Display the entire greenhouse structure with all plants
     */
    void displayGreenhouseStructure();
    
    /**
     * @brief Display plants by their current state
     * @param state State name (e.g., "InNursery", "Planted", "Growing")
     */
    void displayPlantsByState(const std::string& state);
    
    /**
     * @brief Get count of plants in greenhouse structure
     * @return Total number of plants
     */
    int getTotalPlantCount() const;
    
    /**
     * @brief Get greenhouse root
     * @return Root component
     */
    GreenhouseComponent* getRoot() const { return root; }
    
    /**
     * @brief Register a species node for quick lookup
     * @param speciesName Name of the species
     * @param node The PlantGroup node representing this species
     */
    void registerSpeciesNode(const std::string& speciesName, PlantGroup* node);
    
private:
    /**
     * @brief Helper to find species node by name (if not cached)
     */
    PlantGroup* findSpeciesNode(const std::string& speciesName);
};

#endif // GREENHOUSEMANAGER_H
