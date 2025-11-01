#ifndef FLOOD_WATERING_STRATEGY_H
#define FLOOD_WATERING_STRATEGY_H

#include "CareStrategy.h"
#include <iostream>

class FloodWateringStrategy : public CareStrategy
{
public:
    void applyCare(int amount, const std::string &careType) override
    {
        if (careType == "flood")
        {
            std::cout << "Applying " << amount << " ml of water by Flooding." << std::endl;
        }
        else
        {
            std::cout << "FloodWateringStrategy does not handle care type: " << careType << std::endl;
        }
    }
    std::string getName() const override { return "Flood Watering"; }
};

#endif // FLOOD_WATERING_STRATEGY_H
