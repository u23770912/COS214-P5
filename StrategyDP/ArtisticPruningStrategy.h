#ifndef ARTISTIC_PRUNING_STRATEGY_H
#define ARTISTIC_PRUNING_STRATEGY_H

#include "CareStrategy.h"
#include <iostream>

class ArtisticPruningStrategy : public CareStrategy {
public:
    void execute(const std::string& property) override {
        std::cout << "Pruning artistically based on style: " << property << std::endl;
    }
};

#endif // ARTISTIC_PRUNING_STRATEGY_H
