#ifndef WATERING_STRATEGY_H
#define WATERING_STRATEGY_H

#include "CareStrategy.h"

// Strategy Interface (Strategy Pattern)
// Defines an algorithm for applying water.
class WateringStrategy : public CareStrategy {
public:
    // virtual ~WateringStrategy() {}
    void applyCare(int amount, const std::string& careType) override;
};

#endif // WATERING_STRATEGY_H
