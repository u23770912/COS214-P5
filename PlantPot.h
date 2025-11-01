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
    void print(const std::string& prefix , bool isLast) override;
    
    void remove(GreenhouseComponent*) override;
    void display() override;
    void getPlantCount() override;
};


#endif