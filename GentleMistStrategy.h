#ifndef GENTLEMISTSTRATEGY_H
#define GENTLEMISTSTRATEGY_H

#include "CareStrategy.h"
#include <iostream>

/**
 * @class GentleMistStrategy
 * @brief Implements a misting algorithm for humidity-loving plants.
 * 
 * This strategy is used to increase local humidity, which is crucial for
 * certain tropical plants or ferns.
 */
class GentleMistStrategy : public CareStrategy {
public:
void applyCare(int amount, const std::string& careType) override {
        if (careType == "mist") {
            std::cout << "Gently misting to maintain humidity level: " << amount << " ml" << std::endl;
        } else {
            std::cout << "GentleMistStrategy does not handle care type: " << careType << std::endl;
        }
    }
std::string getName() const override { return "Gentle Mist"; }
};

#endif // GENTLEMISTSTRATEGY_H
