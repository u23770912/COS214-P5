#ifndef DRIP_WATERING_STRATEGY_H
#define DRIP_WATERING_STRATEGY_H

#include "CareStrategy.h"
#include <iostream>

class DripWateringStrategy : public CareStrategy
{
public:
    void applyCare(int amount, const std::string &careType) override
    {
        if (careType == "drip" || careType == "water" || careType == "watering")
        {
            std::cout << "Applying " << amount << " ml of water via Drip System." << std::endl;
        }
        else
        {
            std::cout << "DripWateringStrategy does not handle care type: " << careType << std::endl;
        }
    }

    std::string getName() const override { return "Drip Watering"; }
};

#endif // DRIP_WATERING_STRATEGY_H
