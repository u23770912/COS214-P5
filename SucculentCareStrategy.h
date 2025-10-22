#ifndef SUCCULENTCARESTRATEGY_H
#define SUCCULENTCARESTRATEGY_H

#include "CareStrategy.h"
#include "PlantProduct.h"
#include <iostream>

/**
 * @class SucculentWateringStrategy
 * @brief Implements the watering strategy specifically for succulents.
 */
class SucculentWateringStrategy : public CareStrategy {
public:
    void execute(PlantProduct* plant) const override {
        std::cout << "Watering succulent " << plant->getProperty("species") 
                  << " sparingly, allowing soil to dry out completely." << std::endl;
        // Logic for succulent watering (infrequent, deep watering)
    }
};

/**
 * @class SucculentPruningStrategy
 * @brief Implements the pruning strategy specifically for succulents.
 */
class SucculentPruningStrategy : public CareStrategy {
public:
    void execute(PlantProduct* plant) const override {
        std::cout << "Pruning succulent " << plant->getProperty("species") 
                  << " by removing dead leaves from the base." << std::endl;
        // Logic for succulent pruning (minimal, removing dead parts)
    }
};

#endif // SUCCULENTCARESTRATEGY_H
