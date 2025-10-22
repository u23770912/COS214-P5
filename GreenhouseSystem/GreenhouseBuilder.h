#ifndef GREENHOUSEBUILDER_H
#define GREENHOUSEBUILDER_H

#include <string>
#include "GreenhouseComponent.h"

class GreenhouseBuilder {
public:
    virtual ~GreenhouseBuilder() = default;
    virtual void reset() = 0;
    virtual void addCategory(std::string name) = 0;
    virtual void addType(std::string categoryName, std::string typeName) = 0;
    virtual void addPlant(std::string typeName, std::string plantId, std::string plantInfo) = 0;
    virtual GreenhouseComponent* getGreenhouse() = 0;
};

#endif
