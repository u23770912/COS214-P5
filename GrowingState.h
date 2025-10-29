#ifndef GROWING_STATE_H
#define GROWING_STATE_H

#include "PlantState.h"
#include <string>

class GrowingState : public PlantState {
private:
    int careCount;
    
public:
    GrowingState() : careCount(0) {}
    
    void onEnter(PlantProduct* plant) override;
    void onExit(PlantProduct* plant) override;
    std::string getName() const override { return "Growing"; }
    void advanceState(PlantProduct* plant) override;
};

#endif