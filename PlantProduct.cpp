#include "PlantProduct.h"
#include "StateDP/PlantState.h"
#include "BridgeDP/PlantSpeciesProfile.h"
#include "StrategyDP/CareStrategy.h"
#include "StateDP/WitheringState.h"
#include <iostream>

PlantProduct::PlantProduct(const std::string &id, PlantSpeciesProfile *profile)
    : currentState(nullptr), monitor(nullptr), speciesProfile(profile), plantId(id)
{
    // Initial state could be set here, e.g., transitionTo(new PlantedState());
}

PlantProduct::~PlantProduct()
{
    delete currentState;
    delete speciesProfile;
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

void PlantProduct::performCare(const std::string &careType)
{
    auto it = strategy_map.find(careType);
    if (it != strategy_map.end())
    {
        std::cout << "Performing '" << careType << "' care for " << speciesProfile->getSpeciesName() << "." << std::endl;
        // Pass a relevant property from the profile to the strategy
        std::string property = speciesProfile->getProperty("some_property_for_" + careType);
        it->second->execute(property);
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
        currentState->advanceState(this);
    }
}