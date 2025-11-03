#include "GrowingState.h"
#include "PlantProduct.h"
#include "ReadyForSaleState.h"
#include "PlantSpeciesProfile.h"
#include <iostream>

void GrowingState::onEnter(PlantProduct *plant)
{
    PlantSpeciesProfile *profile = plant->getProfile();
    int duration = profile ? profile->getStateDurationSeconds("Growing", 20) : 20;

    std::cout << "[STATE] Plant entered Growing state (" << duration << " seconds)" << std::endl;
    careCount = 0;
}

void GrowingState::onExit(PlantProduct *plant)
{
    std::cout << "[STATE] Plant exiting Growing state" << std::endl;
}

void GrowingState::advanceState(PlantProduct *plant)
{
    int secondsInState = plant->getSecondsInCurrentState();
    int secondsSinceCare = plant->getSecondsSinceLastCare();

    PlantSpeciesProfile *profile = plant->getProfile();

    // Get intervals from profile (defaults if not set)
    int wateringInterval = 10;
    int pruningInterval = 12;
    if (profile)
    {
        wateringInterval = profile->getCareIntervalSeconds("Watering", wateringInterval);
        pruningInterval = profile->getCareIntervalSeconds("Pruning", pruningInterval);
    }

    // Determine which interval to use based on care type
    int targetInterval = (careCount % 2 == 0) ? wateringInterval : pruningInterval;

    // Request care when interval is reached
    if (secondsSinceCare >= targetInterval)
    {
        if (careCount % 2 == 0)
        {
            std::cout << "[GROWING] Requesting water (interval: " << wateringInterval << "s)..." << std::endl;
            plant->notify("Watering");
        }
        else
        {
            std::cout << "[GROWING] Requesting pruning (interval: " << pruningInterval << "s)..." << std::endl;
            plant->notify("Pruning");
        }
        careCount++;
        plant->resetLastCareTime();
    }

    // Get state duration from profile
    int growingDuration = 20;
    if (profile)
    {
        growingDuration = profile->getStateDurationSeconds("Growing", growingDuration);
    }

    // Advance to next state when duration is complete
    if (secondsInState >= growingDuration)
    {
        std::cout << "[GROWING] Plant mature. Moving to ReadyForSale." << std::endl;
        plant->transitionTo(new ReadyForSaleState());
    }
}