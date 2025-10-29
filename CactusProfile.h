#ifndef CACTUS_PROFILE_H
#define CACTUS_PROFILE_H

#include "PlantSpeciesProfile.h"

// In CactusProfile.h
class CactusProfile : public PlantSpeciesProfile {
public:
    CactusProfile() {
        properties["category"] = "Cactus";
        properties["species"] = "Cactus";
        properties["idealWater"] = "50ml";
        properties["idealSunlight"] = "Full Sun";
        properties["idealSoil"] = "Sandy";
        properties["preferredWaterMethod"] = "drip";  
        properties["needsPruning"] = "false";        
        properties["sensitiveTo"] = "mist,flood";     
        properties["daysInNursery"] = "4";
        properties["daysPlantedToReady"] = "6"; 
        properties["maxDaysWithoutCare"] = "10";
    }
    std::string getSpeciesName() const override { return "Cactus"; }
};

#endif // CACTUS_PROFILE_H