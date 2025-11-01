#ifndef GREENHOUSESYSTEM_H
#define GREENHOUSESYSTEM_H

#include <string>
#include "GreenhouseBuilder.h"

#include "GreenhouseComponent.h"
#include "PlantGroup.h"
#include <map> 

class GreenhouseSystem : public GreenhouseBuilder
{

    private: 
        GreenhouseComponent* root;
        GreenhouseComponent* currentCategory;
        // std::map<std::string, PlantGroup*> categories;
        // std::map<std::string, PlantGroup*> types;

    public:
        GreenhouseSystem();
        ~GreenhouseSystem(); 
        void reset() override;
        GreenhouseBuilder* addCategory(std::string name) override;
        GreenhouseBuilder* addType(std::string name) override;
        GreenhouseBuilder* addSpecies(std::string name) override;

        // void addPlant(std::string id, std::string info) override;
        GreenhouseComponent* getGreenhouse() override;

        GreenhouseBuilder* addPlant(PlantData* plant) override; // New function

};


#endif