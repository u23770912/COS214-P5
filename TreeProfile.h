#ifndef TREEPROFILE_H
#define TREEPROFILE_H

#include "PlantSpeciesProfile.h"
#include <string>

/**
 * @class TreeProfile
 * @brief A concrete profile for all tree-type plants.
 * 
 * This class holds the data for a specific type of tree (e.g., Bonsai),
 * which is provided to its constructor at runtime.
 */
class TreeProfile : public PlantSpeciesProfile {
public:
    TreeProfile(const std::string& speciesName, const std::string& water, 
                const std::string& sunlight, const std::string& soil) {
        properties["category"] = "Tree";
        properties["species"] = speciesName;
        properties["idealWater"] = water;
        properties["idealSunlight"] = sunlight;
        properties["idealSoil"] = soil;
        properties["preferredWaterMethod"] = "drip";  // Trees need deep watering
        properties["needsPruning"] = "true";
        properties["pruningStyle"] = "artistic";
        properties["daysInNursery"] = "5";
        properties["daysPlantedToReady"] = "7";
        properties["maxDaysWithoutCare"] = "9";
    }

    std::string getSpeciesName() const override {
        auto it = properties.find("species");
        return it != properties.end() ? it->second : "Unknown Tree";
    }
};

#endif // TREEPROFILE_H
