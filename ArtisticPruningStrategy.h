#ifndef ARTISTIC_PRUNING_STRATEGY_H
#define ARTISTIC_PRUNING_STRATEGY_H

#include "CareStrategy.h"
#include <iostream>

class ArtisticPruningStrategy : public CareStrategy
{
public:
    void applyCare(int amount, const std::string &careType) override
    {
        if (careType == "prune_artistic")
        {
            std::cout << "Pruning artistically based on style with " << amount << " cuts." << std::endl;
        }
        else
        {
            std::cout << "ArtisticPruningStrategy does not handle care type: " << careType << std::endl;
        }
    }
    std::string getName() const override { return "Artistic Pruning"; }
};

#endif // ARTISTIC_PRUNING_STRATEGY_H
