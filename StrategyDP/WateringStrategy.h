#ifndef WATERING_STRATEGY_H
#define WATERING_STRATEGY_H

// Strategy Interface (Strategy Pattern)
// Defines an algorithm for applying water.
class WateringStrategy {
public:
    virtual ~WateringStrategy() {}
    virtual void applyWater(int amount) = 0;
};

#endif // WATERING_STRATEGY_H
