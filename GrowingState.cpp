#include "GrowingState.h"
#include "PlantProduct.h"
#include "ReadyForSaleState.h"
#include <iostream>

void GrowingState::onEnter(PlantProduct* plant) {
    std::cout << "[STATE] Plant entered Growing state (90 seconds)" << std::endl;
    careCount = 0;
}

void GrowingState::onExit(PlantProduct* plant) {
    std::cout << "[STATE] Plant exiting Growing state" << std::endl;
}

void GrowingState::advanceState(PlantProduct* plant) {
    int secondsInState = plant->getSecondsInCurrentState();
    int secondsSinceCare = plant->getSecondsSinceLastCare();

    if (secondsSinceCare >= 5) {
        if (careCount % 2 == 0) {
            std::cout << "[GROWING] Requesting water..." << std::endl;
            plant->notify("Watering");
        } else {
            std::cout << "[GROWING] Requesting pruning..." << std::endl;
            plant->notify("Pruning");
        }
        careCount++;
        plant->resetLastCareTime();
    }

    if (secondsInState >= 20) {
        std::cout << "[GROWING] Plant mature. Moving to ReadyForSale." << std::endl;
        plant->transitionTo(new ReadyForSaleState());
    }
}
