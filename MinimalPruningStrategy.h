#ifndef MINIMAL_PRUNING_STRATEGY_H
#define MINIMAL_PRUNING_STRATEGY_H
#include <iostream>

#include "CareStrategy.h"

class MinimalPruningStrategy : public CareStrategy
{
public:
    virtual void applyCare(int amount, const std::string &careType) override
    {
        // Implementation of minimal pruning logic
        std::cout << "Applying minimal pruning of amount " << amount << " for care type: " << careType << std::endl;
    }
    std::string getName() const override;
};

#endif // MINIMAL_PRUNING_STRATEGY_H
