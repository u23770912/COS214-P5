#ifndef PLANT_SPECIES_PROFILE_H
#define PLANT_SPECIES_PROFILE_H

#include <string>
#include <map>

// Implementor Interface (Bridge Pattern)
// Defines the biological properties of a plant species using a generic property map.
class PlantSpeciesProfile {
protected:
    std::map<std::string, std::string> properties;

public:
    virtual ~PlantSpeciesProfile() {}

    std::string getProperty(const std::string& key) const {
        auto it = properties.find(key);
        return it != properties.end() ? it->second : "N/A";
    }

    virtual std::string getSpeciesName() const = 0;
};

#endif // PLANT_SPECIES_PROFILE_H
