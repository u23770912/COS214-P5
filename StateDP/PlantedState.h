#ifndef PLANTED_STATE_H
#define PLANTED_STATE_H

#include "PlantState.h"
#include <string>

class PlantedState : public PlantState {
public:
    void advanceState(PlantProduct* plant) override;
    void onEnter(PlantProduct* plant) override {}
    void onExit(PlantProduct* plant) override {}
    std::string getName() const override { return "Planted"; }
};

#endif // PLANTED_STATE_H
