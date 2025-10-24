#ifndef CONCRETE_STRATEGIES_H
#define CONCRETE_STRATEGIES_H

#include "WateringStrategy.h"
#include <iostream>

// --- Concrete Strategies ---

class DripWatering : public WateringStrategy {
public:
    void applyWater(int amount) override {
        std::cout << "Applying " << amount << "ml of water via Drip System." << std::endl;
    }
};

class FloodWatering : public WateringStrategy {
public:
    void applyWater(int amount) override {
        std::cout << "Applying " << amount << "ml of water by Flooding." << std::endl;
    }
};

// We can add other strategy types here too
class PruningStrategy {
public:
    virtual ~PruningStrategy() {}
    virtual void prune() = 0;
};

class ArtisticPruning : public PruningStrategy {
public:
    void prune() override {
        std::cout << "Pruning artistically (e.g., for a Bonsai)." << std::endl;
    }
};


#endif // CONCRETE_STRATEGIES_H
