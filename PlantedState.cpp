#include "PlantedState.h"
#include "PlantProduct.h"
#include "InNurseryState.h"
#include "PlantSpeciesProfile.h"
#include <iostream>

void PlantedState::onEnter(PlantProduct* plant) {
    PlantSpeciesProfile* profile = plant->getProfile();
    int duration = profile ? profile->getStateDurationSeconds("Planted", 20) : 20;
    
    std::cout << "[STATE] Plant entered Planted state (" << duration << " seconds)" << std::endl;
}

void PlantedState::onExit(PlantProduct* plant) {
    std::cout << "[STATE] Plant exiting Planted state" << std::endl;
}

void PlantedState::advanceState(PlantProduct* plant) {
    int secondsInState = plant->getSecondsInCurrentState();
    int secondsSinceCare = plant->getSecondsSinceLastCare();

    // Get watering interval from profile
    int waterInterval = 10;
    if (PlantSpeciesProfile* profile = plant->getProfile()) {
        waterInterval = profile->getCareIntervalSeconds("Watering", waterInterval);
    }

    // Request water at appropriate interval
    if (secondsSinceCare >= waterInterval) {
        std::cout << "[PLANTED] Requesting water (interval: " << waterInterval << "s)..." << std::endl;
        plant->notify("Watering");
        plant->resetLastCareTime();
    }

    // Get state duration from profile
    int plantedDuration = 20;
    if (PlantSpeciesProfile* profile = plant->getProfile()) {
        plantedDuration = profile->getStateDurationSeconds("Planted", plantedDuration);
    }

    // Advance to next state when duration is complete
    if (secondsInState >= plantedDuration) {
        std::cout << "[PLANTED] Growth stage complete. Moving to InNursery." << std::endl;
        plant->transitionTo(new InNurseryState());
    }
}