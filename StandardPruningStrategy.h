#ifndef STANDARDPRUNINGSTRATEGY_H
#define STANDARDPRUNINGSTRATEGY_H

#include "CareStrategy.h"
#include <iostream>

/**
 * @class StandardPruningStrategy
 * @brief Implements a standard pruning algorithm for general plant health.
 * 
 * This involves removing dead, damaged, or overgrown branches to encourage
 * healthy growth.
 */
class StandardPruningStrategy : public CareStrategy {
public:
void applyCare(int amount, const std::string& careType) override {
        if (careType == "prune_standard") {
            std::cout << "Performing standard pruning focusing on " << amount << " dead leaves and branches." << std::endl;
        } else {
            std::cout << "StandardPruningStrategy does not handle care type: " << careType << std::endl;
        }
    }
};

#endif // STANDARDPRUNINGSTRATEGY_H
