#ifndef SUCCULENTPROFILE_H
#define SUCCULENTPROFILE_H

#include "PlantSpeciesProfile.h"
#include <string>

/**
 * @class SucculentProfile
 * @brief A concrete profile for all succulent-type plants.
 * 
 * This class holds the data for a specific type of succulent (e.g., Cactus),
 * which is provided to its constructor at runtime.
 */
class SucculentProfile : public PlantSpeciesProfile {
public:
    SucculentProfile(const std::string& speciesName, const std::string& water, const std::string& sunlight, const std::string& soil) {
        properties["category"] = "Succulent";
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
        return it != properties.end() ? it->second : "Unknown Succulent";
    }
};

#endif // SUCCULENTPROFILE_H
