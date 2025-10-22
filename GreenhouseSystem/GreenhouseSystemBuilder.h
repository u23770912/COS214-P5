#ifndef GREENHOUSESYSTEMBUILDER_H
#define GREENHOUSESYSTEMBUILDER_H

#include "GreenhouseBuilder.h"
#include "GreenhouseComponent.h"
#include "PlantGroup.h"
#include "PlantInstance.h"
#include <string>
#include <map>

// Assuming CareCommands will be defined elsewhere, e.g., WaterCommand.h
// #include "WaterCommand.h" 

class GreenhouseSystemBuilder : public GreenhouseBuilder {
private:
    PlantGroup* root;
    std::map<std::string, PlantGroup*> categories;
    std::map<std::string, PlantGroup*> types;

public:
    GreenhouseSystemBuilder();
    ~GreenhouseSystemBuilder();
    void reset() override;
    void addCategory(std::string name) override;
    void addType(std::string categoryName, std::string typeName) override;
    void addPlant(std::string typeName, std::string plantId, std::string plantInfo) override;
    GreenhouseComponent* getGreenhouse() override;
};

#endif
