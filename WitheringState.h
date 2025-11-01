#ifndef WITHERINGSTATE_H
#define WITHERINGSTATE_H

#include "PlantState.h"

/**
 * @class WitheringState
 * @brief Represents the state of a plant that is neglected and withering.
 * 
 * This is a terminal state in this design. A plant that enters this state
 * cannot be recovered.
 */
class WitheringState : public PlantState {
public:
    void onEnter(PlantProduct* plant) override;
    void onExit(PlantProduct* plant) override;
    std::string getName() const override;
    void advanceState(PlantProduct* plant) override;
};

#endif // WITHERINGSTATE_H