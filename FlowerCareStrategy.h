#ifndef FLOWERCARESTRATEGY_H
#define FLOWERCARESTRATEGY_H

#include "CareStrategy.h"
#include "PlantProduct.h"
#include <iostream>

/**
 * @class FlowerWateringStrategy
 * @brief Implements the watering strategy specifically for flowers.
 */
class FlowerWateringStrategy : public CareStrategy {
public:
    void execute(PlantProduct* plant) const override {
        std::cout << "Watering flower " << plant->getProperty("species") 
                  << " with a gentle shower, as per flower care standards." << std::endl;
        // Logic for flower watering would go here
    }
};

/**
 * @class FlowerPruningStrategy
 * @brief Implements the pruning strategy specifically for flowers.
 */
class FlowerPruningStrategy : public CareStrategy {
public:
    void execute(PlantProduct* plant) const override {
        std::cout << "Pruning flower " << plant->getProperty("species") 
                  << " by deadheading spent blooms." << std::endl;
        // Logic for flower pruning would go here
    }
};

#endif // FLOWERCARESTRATEGY_H
