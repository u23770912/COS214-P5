#include "InNurseryState.h"
#include "PlantProduct.h"
#include "GrowingState.h"
#include <iostream>

void InNurseryState::onEnter(PlantProduct* plant) {
    std::cout << "[STATE] Plant entered InNursery state (45 seconds)" << std::endl;
    lastWasWater = false;
}

void InNurseryState::onExit(PlantProduct* plant) {
    std::cout << "[STATE] Plant exiting InNursery state" << std::endl;
}

void InNurseryState::advanceState(PlantProduct* plant) {
    int secondsInState = plant->getSecondsInCurrentState();
    int secondsSinceCare = plant->getSecondsSinceLastCare();

    if (secondsSinceCare >= 5) {
        if (lastWasWater) {
            std::cout << "[IN_NURSERY] Requesting fertilizer..." << std::endl;
            plant->notify("Fertilizing");
            lastWasWater = false;
        } else {
            std::cout << "[IN_NURSERY] Requesting water..." << std::endl;
            plant->notify("Watering");
            lastWasWater = true;
        }
        plant->resetLastCareTime();
    }

    if (secondsInState >= 15) {
        std::cout << "[IN_NURSERY] Growth stage complete. Moving to Growing." << std::endl;
        plant->transitionTo(new GrowingState());
    }
}