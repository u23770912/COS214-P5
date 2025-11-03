#ifndef GREENHOUSEDIRECTOR_H
#define GREENHOUSEDIRECTOR_H

#include <string>
#include <vector>

#include "GreenhouseBuilder.h"
#include "PlantData.h"
#include "GreenhouseComponent.h"
#include "GreenhouseSystem.h"

class GreenhouseDirector
{
    private: 
        GreenhouseBuilder* builder;
        std::vector<PlantData*> plants;

    public:
        GreenhouseDirector(GreenhouseBuilder* builder);
        // ~GreenhouseDirector();
        GreenhouseComponent* construct();
        std::vector<PlantData*> getPlants();
        
        void setPlants(std::vector<PlantData*> plants);
};


#endif