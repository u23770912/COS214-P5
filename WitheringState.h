#ifndef WITHERINGSTATE_H
#define WITHERINGSTATE_H

#include "PlantState.h"
#include "PlantProduct.h"
#include <iostream>

/**
 * @class WitheringState
 * @brief Represents the state of a plant that is neglected and withering.
 * 
 * This is a terminal state in this design. A plant that enters this state
 * cannot be recovered.
 */
class WitheringState : public PlantState {
public:
    void onEnter(PlantProduct* plant) override {
        if (plant && plant->getProfile()) {
            std::cout << "Plant " << plant->getProfile()->getSpeciesName() << " is now withering due to neglect." << std::endl;
        } else {
            std::cout << "A plant has entered the withering state due to neglect." << std::endl;
        }
    }

    void onExit(PlantProduct* plant) override {
        // Typically no action needed when exiting a terminal state.
    }

    std::string getName() const override {
        return "Withering";
    }
};

#endif // WITHERINGSTATE_H
