#ifndef ROSE_PROFILE_H
#define ROSE_PROFILE_H

#include "PlantSpeciesProfile.h"

class RoseProfile : public PlantSpeciesProfile {
public:
    RoseProfile() {
        properties["Watering"] = "500ml";
        properties["Fertilizing"] = "RoseFood";
        properties["Pruning"] = "RemoveDeadHeads";
    }
    std::string getSpeciesName() const override { return "Rose"; }
};

#endif // ROSE_PROFILE_H
