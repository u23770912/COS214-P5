#ifndef DRIP_WATERING_STRATEGY_H
#define DRIP_WATERING_STRATEGY_H

#include "CareStrategy.h"
#include <iostream>

/**
 * @brief Strategy for drip watering method
 * 
 * Implements a slow, controlled watering strategy that delivers
 * water directly to plant roots through a drip system.
 */
class DripWateringStrategy : public CareStrategy
{
    public:
    
        void applyCare(int amount, const std::string &careType) override;

        std::string getName() const override;
};

#endif // DRIP_WATERING_STRATEGY_H
