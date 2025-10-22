#ifndef GREENHOUSEDIRECTOR_H
#define GREENHOUSEDIRECTOR_H

#include "GreenhouseBuilder.h"
#include "PlantData.h"
#include "GreenhouseComponent.h"
#include <vector>
#include <string>

class GreenhouseDirector {
private:
    GreenhouseBuilder* builder;
    std::vector<PlantData*> plants;

public:
    GreenhouseDirector(GreenhouseBuilder* b);
    ~GreenhouseDirector();
    void setPlants(std::vector<PlantData*> p);
    GreenhouseComponent* construct();
};

#endif
