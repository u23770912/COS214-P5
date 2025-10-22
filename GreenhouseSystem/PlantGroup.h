#ifndef PLANTGROUP_H
#define PLANTGROUP_H

#include "GreenhouseComponent.h"
#include <vector>
#include <string>

class PlantGroup : public GreenhouseComponent {
private:
    std::string name;
    std::vector<GreenhouseComponent*> children;

public:
    PlantGroup(std::string name);
    ~PlantGroup();
    std::string getName();
    void setName(std::string name);
    void add(GreenhouseComponent* item) override;
    void print() override;
};

#endif
