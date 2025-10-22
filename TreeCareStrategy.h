#ifndef TREECARESTRATEGY_H
#define TREECARESTRATEGY_H

#include "CareStrategy.h"
#include "PlantProduct.h"
#include <iostream>

/**
 * @class TreeWateringStrategy
 * @brief Implements the watering strategy specifically for trees.
 */
class TreeWateringStrategy : public CareStrategy {
public:
    void execute(PlantProduct* plant) const override {
        std::cout << "Watering tree " << plant->getProperty("species") 
                  << " with a deep soak to encourage root growth." << std::endl;
        // Logic for tree watering
    }
};

/**
 * @class TreePruningStrategy
 * @brief Implements the pruning strategy specifically for trees.
 */
class TreePruningStrategy : public CareStrategy {
public:
    void execute(PlantProduct* plant) const override {
        std::cout << "Pruning tree " << plant->getProperty("species") 
                  << " to shape and remove crossing branches." << std::endl;
        // Logic for tree pruning (structural)
    }
};

#endif // TREECARESTRATEGY_H
