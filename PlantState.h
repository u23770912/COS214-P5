#ifndef PLANT_STATE_H
#define PLANT_STATE_H
#include <iostream>

// Forward declaration
class PlantProduct;

// Abstract State (State Pattern)
class PlantState {
public:
    virtual ~PlantState() {}
    virtual void advanceState(PlantProduct* plant) = 0;
    virtual void onEnter(PlantProduct* plant) = 0;
    virtual void onExit(PlantProduct* plant) = 0;
    void getLifeCycleMonitor();
    virtual std::string getName() const = 0;
};

#endif // PLANT_STATE_H
