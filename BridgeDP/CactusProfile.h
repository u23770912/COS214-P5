#ifndef CACTUS_PROFILE_H
#define CACTUS_PROFILE_H

#include "PlantSpeciesProfile.h"

class CactusProfile : public PlantSpeciesProfile {
public:
    CactusProfile() {
        properties["Watering"] = "50ml";
        properties["Fertilizing"] = "None";
        properties["Pruning"] = "None";
    }
    std::string getSpeciesName() const override { return "Cactus"; }
};

#endif // CACTUS_PROFILE_H
