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
        /**
     * @brief Parameterized constructor
     * @param id Plant ID
     * @param info Description
     * @param type Type of plant
     * @param category Category name
     * @param price Unit price
     * @param stockQuantity Quantity in stock
     */
        ~PlantGroup();
        std::string getName();

        void setName(std::string name);
        
        void add(GreenhouseComponent* item) override;
        void print(const std::string& prefix , bool isLast) override;
        void display() override;
        void getPlantCount() override;
        void remove(GreenhouseComponent* item) override;
        void removeWithoutDelete(GreenhouseComponent* item);  // Remove from vector without deleting
};

#endif