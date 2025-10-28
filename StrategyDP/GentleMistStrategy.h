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
    void execute(const std::string& property) override {
        std::cout << "Gently misting to maintain humidity level: " << property << std::endl;
    }
};

#endif // GENTLEMISTSTRATEGY_H
