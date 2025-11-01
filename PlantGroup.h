#ifndef PLANTGROUP_H
#define PLANTGROUP_H

#include <string>
#include <vector> 
#include <algorithm>

#include "GreenhouseComponent.h"

class PlantGroup: public GreenhouseComponent {
    private:
        std::string name;
        std::vector<GreenhouseComponent*> children;
        void printRecursive(const std::string& prefix, bool isLast); 

    public: 
        PlantGroup(std::string name);
        ~PlantGroup();
        std::string getName();

        void setName(std::string name);
        
        void add(GreenhouseComponent* item) override;
        void print() override;
        void display() override;
        void getPlantCount() override;
        void remove(GreenhouseComponent* item) override;
};

#endif