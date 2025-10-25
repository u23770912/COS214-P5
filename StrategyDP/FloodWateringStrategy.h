#ifndef FLOOD_WATERING_STRATEGY_H
#define FLOOD_WATERING_STRATEGY_H

#include "CareStrategy.h"
#include <iostream>

class FloodWateringStrategy : public CareStrategy {
public:
    void execute(const std::string& property) override {
        std::cout << "Applying " << property << " of water by Flooding." << std::endl;
    }
};

#endif // FLOOD_WATERING_STRATEGY_H
