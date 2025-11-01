#ifndef PLANTPOT_H
#define PLANTPOT_H

#include "GreenhouseComponent.h"

class PlantPot : public GreenhouseComponent
{
private:
    std::string id;
    std::string info;


public:
    PlantPot(std::string id, std::string info);
    ~PlantPot();
    std::string getId();
    std::string getInfo();

    void add(GreenhouseComponent*) override; 
    void print() override;
};


#endif