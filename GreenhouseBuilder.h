
#ifndef GREENHOUSEBUILDER_H
#define GREENHOUSEBUILDER_H

#include <string>

#include "GreenhouseComponent.h"

class GreenhouseBuilder
{

public:

    virtual ~GreenhouseBuilder();

    virtual void reset() = 0;
    virtual GreenhouseBuilder* addCategory(std::string name) = 0;
    virtual GreenhouseBuilder* addType(std::string name) = 0;
    virtual GreenhouseBuilder* addSpecies(std::string name) = 0;
    // virtual void addPlant(std::string id, std::string info) = 0; // in case not using composite, this is to add the leaf
    virtual GreenhouseComponent* getGreenhouse() = 0;

};

#endif




// #ifndef GREENHOUSEBUILDER_H
// #define GREENHOUSEBUILDER_H

// #include <string>

// #include "GreenhouseComponent.h"

// class GreenhouseBuilder
// {

// public:

//     virtual ~GreenhouseBuilder();

//     virtual void reset() = 0;
//     virtual GreenhouseBuilder* addCategory(std::string name) = 0;
//     virtual GreenhouseBuilder* addType(std::string type) = 0;
//     virtual PlantProduct* addSpecies(std::string type) = 0;
//     // virtual void addPlant(std::string id, std::string info) = 0; // in case not using composite, this is to add the leaf
//     virtual GreenhouseComponent* getGreenhouse() = 0;

// };

// #endif