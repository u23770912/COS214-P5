#include "InNurseryState.h"
#include "PlantProduct.h"
#include "GrowingState.h"
#include "PlantSpeciesProfile.h"
#include <iostream>

void InNurseryState::onEnter(PlantProduct* plant) {
    PlantSpeciesProfile* profile = plant->getProfile();
    int duration = profile ? profile->getStateDurationSeconds("InNursery", 20) : 20;
    
    std::cout << "[STATE] Plant entered InNursery state (" << duration << " seconds)" << std::endl;
    lastWasWater = false;
}

void InNurseryState::onExit(PlantProduct* plant) {
    std::cout << "[STATE] Plant exiting InNursery state" << std::endl;
}

void InNurseryState::advanceState(PlantProduct* plant) {
    int secondsInState = plant->getSecondsInCurrentState();
    int secondsSinceCare = plant->getSecondsSinceLastCare();

    PlantSpeciesProfile* profile = plant->getProfile();
    
    // Get appropriate interval based on what care is needed next
    int requestInterval = 10;
    if (profile) {
        if (lastWasWater) {
            requestInterval = profile->getCareIntervalSeconds("Fertilizing", requestInterval);
        } else {
            requestInterval = profile->getCareIntervalSeconds("Watering", requestInterval);
        }
    }

    // Alternate between watering and fertilizing
    if (secondsSinceCare >= requestInterval) {
        if (lastWasWater) {
            std::cout << "[IN_NURSERY] Requesting fertilizer (interval: " << requestInterval << "s)..." << std::endl;
            plant->notify("Fertilizing");
            lastWasWater = false;
        } else {
            std::cout << "[IN_NURSERY] Requesting water (interval: " << requestInterval << "s)..." << std::endl;
            plant->notify("Watering");
            lastWasWater = true;
        }
        plant->resetLastCareTime();
    }

    // Get state duration from profile
    int nurseryDuration = 20;
    if (profile) {
        nurseryDuration = profile->getStateDurationSeconds("InNursery", nurseryDuration);
    }

    // Advance to next state when duration is complete
    if (secondsInState >= nurseryDuration) {
        std::cout << "[IN_NURSERY] Growth stage complete. Moving to Growing." << std::endl;
        plant->transitionTo(new GrowingState());
    }
}