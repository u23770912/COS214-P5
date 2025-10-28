#include "PlantedState.h"
#include "PlantProduct.h"
#include "InNurseryState.h"
#include <iostream>

void PlantedState::onEnter(PlantProduct* plant) {
    std::cout << "[STATE] Plant entered Planted state (30 seconds)" << std::endl;
}

void PlantedState::onExit(PlantProduct* plant) {
    std::cout << "[STATE] Plant exiting Planted state" << std::endl;
}

void PlantedState::advanceState(PlantProduct* plant) {
    int secondsInState = plant->getSecondsInCurrentState();
    int secondsSinceCare = plant->getSecondsSinceLastCare();

    if (secondsSinceCare >= 3) {
        std::cout << "[PLANTED] Requesting water..." << std::endl;
        plant->notify("Watering");
        plant->resetLastCareTime();
    }

    if (secondsInState >= 10) {
        std::cout << "[PLANTED] Growth stage complete. Moving to InNursery." << std::endl;
        plant->transitionTo(new InNurseryState());
    }
}

