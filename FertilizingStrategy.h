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
void applyCare(int amount, const std::string& careType) override {
        if (careType == "fertilize") {
            std::cout << "Applying " << amount << " grams of fertilizer blend to replenish nutrients." << std::endl;
        } else {
            std::cout << "FertilizingStrategy does not handle care type: " << careType << std::endl;
        }
    }
    std::string getName() const override { return "Fertilizing"; }
};

#endif // FERTILIZINGSTRATEGY_H
