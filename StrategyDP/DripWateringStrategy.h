#ifndef DRIP_WATERING_STRATEGY_H
#define DRIP_WATERING_STRATEGY_H

#include "CareStrategy.h"
#include <iostream>

class DripWateringStrategy : public CareStrategy {
public:
    void execute(const std::string& property) override {
        std::cout << "Applying " << property << " of water via Drip System." << std::endl;
    }
};

#endif // DRIP_WATERING_STRATEGY_H
