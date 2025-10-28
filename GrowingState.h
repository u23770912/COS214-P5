#ifndef GROWING_STATE_H
#define GROWING_STATE_H

#include "PlantState.h"

class GrowingState : public PlantState {
private:
    int careCount = 0;

public:
    void onEnter(PlantProduct* plant) override;
    void onExit(PlantProduct* plant) override;
    std::string getName() const override { return "Growing"; }
    void advanceState(PlantProduct* plant) override;
};

#endif // GROWING_STATE_H
