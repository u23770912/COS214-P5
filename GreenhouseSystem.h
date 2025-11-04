#ifndef GREENHOUSESYSTEM_H
#define GREENHOUSESYSTEM_H
/**
 * @file GreenhouseSystem.h
 * @brief = a  file that is the consctuct part to construct the builder anc complete plants
 */
#include <string>
#include "GreenhouseBuilder.h"

#include "GreenhouseComponent.h"
#include "PlantGroup.h"
#include <map> 

class PlantData;

class GreenhouseSystem : public GreenhouseBuilder
{

    private: 
        GreenhouseComponent* root;
        GreenhouseComponent* currentCategory;
        PlantGroup* lastSpeciesNode;
        // std::map<std::string, PlantGroup*> categories;
        // std::map<std::string, PlantGroup*> types;

    public:
    /**
     * @brief  the constrcut doesn't take any value auses the whole point uis to make a Greenhouse
     */

        GreenhouseSystem();
        ~GreenhouseSystem(); 
        void reset() override;
        GreenhouseBuilder* addCategory(std::string name) override;
        
    /**
     * @brief addCategory is a function that treats objects in a  container a a single9group) and allow to crate a group of plants that have the same session
     * @param component takes ina string and make a group with the follwoing category( with  a name )
     */
        GreenhouseBuilder* addType(std::string name) override;
        GreenhouseBuilder* addSpecies(std::string name) override;
        GreenhouseBuilder* addPlant(PlantData* plant) override;

        // void addPlant(std::string id, std::string info) override;
        GreenhouseComponent* getGreenhouse() override;
        PlantGroup* getLastSpeciesNode();
};


#endif