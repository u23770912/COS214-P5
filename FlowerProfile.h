#ifndef FLOWERPROFILE_H
#define FLOWERPROFILE_H

#include "PlantSpeciesProfile.h"
#include <string>

/**
 * @class FlowerProfile
 * @brief A concrete profile for all flower-type plants.
 * 
 * This class holds the data for a specific type of flower (e.g., Rose),
 * which is provided to its constructor at runtime.
 */
class FlowerProfile : public PlantSpeciesProfile {
public:
    FlowerProfile(const std::string& speciesName, const std::string& water, const std::string& sunlight, const std::string& soil) {
        properties["category"] = "Flower";
        properties["species"] = speciesName;
        properties["idealWater"] = water;
        properties["idealSunlight"] = sunlight;
        properties["idealSoil"] = soil;
        properties["daysInNursery"] = "3";  // Default for testing
        properties["daysPlantedToReady"] = "5";  // Default for testing
        properties["maxDaysWithoutCare"] = "7";  // Default for testing
    }

    std::string getSpeciesName() const override {
        auto it = properties.find("species");
        return it != properties.end() ? it->second : "Unknown Flower";
    }
};

#endif // FLOWERPROFILE_H
