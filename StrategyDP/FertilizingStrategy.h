#ifndef FERTILIZINGSTRATEGY_H
#define FERTILIZINGSTRATEGY_H

#include "CareStrategy.h"
#include <iostream>

/**
 * @class FertilizingStrategy
 * @brief Implements a generic fertilizing algorithm.
 * 
 * This strategy can be applied to provide nutrients to any plant, though the
 * specific fertilizer type could be determined from the plant's profile.
 */
class FertilizingStrategy : public CareStrategy {
public:
    void execute(const std::string& property) override {
        std::cout << "Applying fertilizer blend: " << property
                  << " to replenish nutrients." << std::endl;
    }
};

#endif // FERTILIZINGSTRATEGY_H
