#ifndef IN_NURSERY_STATE_H
#define IN_NURSERY_STATE_H

#include "PlantState.h"
#include <string>

class InNurseryState : public PlantState {
private:
    bool lastWasWater;
    
public:
    InNurseryState() : lastWasWater(false) {}
    
    void onEnter(PlantProduct* plant) override;
    void onExit(PlantProduct* plant) override;
    std::string getName() const override { return "InNursery"; }
    void advanceState(PlantProduct* plant) override;
};

#endif