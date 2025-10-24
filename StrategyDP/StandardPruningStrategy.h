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
    void execute(const std::string& property) override {
        std::cout << "Performing standard pruning focusing on: " << property
                  << " to remove dead leaves and branches." << std::endl;
    }
};

#endif // STANDARDPRUNINGSTRATEGY_H
