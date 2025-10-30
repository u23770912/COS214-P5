//Should be removed
#ifndef GROWING_STATE_H
#define GROWING_STATE_H

#include "PlantState.h"
#include <string>

class GrowingState : public PlantState {
public:
    void onEnter(PlantProduct* plant) override;
    void onExit(PlantProduct* plant) override;
    void advanceState(PlantProduct* plant) override;
    std::string getName() const override { return "Growing"; }
};

#endif // GROWING_STATE_H