#include "PlantProduct.h"
#include "PlantState.h"
#include "InNurseryState.h"
#include "WitheringState.h"
#include "PlantSpeciesProfile.h"
#include "CareStrategy.h"
#include "WateringStrategy.h"
#include "GentleMistStrategy.h"
#include "ArtisticPruningStrategy.h"
#include "FertilizingStrategy.h"
#include "FloodWateringStrategy.h"
#include "StandardPruningStrategy.h"
#include "DripWateringStrategy.h"
#include <iostream>
#include <string>  

PlantProduct::PlantProduct(const std::string& id, PlantSpeciesProfile* profile) 
    : currentState(nullptr), daysInCurrentState(0), monitor(nullptr), speciesProfile(profile), plantId(id) {
    transitionTo(new InNurseryState());
    addStrategy("water", new WateringStrategy());
    addStrategy("mist", new GentleMistStrategy());
    addStrategy("prune_artistic", new ArtisticPruningStrategy());
    addStrategy("fertilize", new FertilizingStrategy());
    addStrategy("flood", new FloodWateringStrategy());
    addStrategy("prune_standard", new StandardPruningStrategy());
    addStrategy("drip", new DripWateringStrategy());
}

PlantProduct::~PlantProduct()
{
    delete currentState;
    // delete speciesProfile;
    for (auto &pair : strategy_map)
    {
        delete pair.second;
    }
}

void PlantProduct::transitionTo(PlantState *state)
{
    if (currentState)
    {
        currentState->onExit(this);
        delete currentState;
    }
    currentState = state;
    currentState->onEnter(this);
    daysInCurrentState = 0; // Reset days when transitioning
}

std::string PlantProduct::getCurrentStateName() const
{
    if (currentState)
    {
        return currentState->getName();
    }
    return "None";
}

void PlantProduct::transitionToWithering()
{
    std::cout << "Transitioning plant to withering state due to an error or neglect." << std::endl;
    transitionTo(new WitheringState());
}

void PlantProduct::notify(const std::string &commandType)
{
    if (monitor)
    {
        monitor->update(this, commandType);
    }
}

PlantSpeciesProfile *PlantProduct::getProfile() const
{
    return speciesProfile;
}

void PlantProduct::addStrategy(const std::string &careType, CareStrategy *strategy)
{
    strategy_map[careType] = strategy;
}

void PlantProduct::performCare(const std::string &careType)//add time
{
    auto it = strategy_map.find(careType);
    if (it != strategy_map.end())
    {
        std::string key;
        if (careType == "water")
            key = "idealWater";
        else if (careType == "mist")
            key = "idealWater"; // Assume mist uses water property
        else if (careType == "prune_artistic")
            key = "idealPruning"; // Add if needed
        else if (careType == "fertilize")
            key = "idealFertilizer"; // Add if needed
        // Add mappings for other care types as needed

        std::string amountStr = speciesProfile->getProperty(key);
        int amount = amountStr.empty() ? 100 : std::stoi(amountStr); // Parse to int, default 100
        std::cout << "Performing '" << careType << "' care for " << speciesProfile->getSpeciesName() << "." << std::endl;
        it->second->applyCare(amount, careType);
    }
    else
    {
        std::cout << "Warning: No strategy found for care type '" << careType << "'." << std::endl;
    }
}

void PlantProduct::advanceLifecycle()
{
    if (currentState)
    {
        // The concrete state's advanceState will handle the transition logic
        daysInCurrentState++;
        currentState->advanceState(this);
    }
}
