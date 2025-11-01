#include "PlantProduct.h"
#include "LifeCycleObserver.h"
#include "PlantState.h"
#include "PlantedState.h"
#include "InNurseryState.h"
#include "GrowingState.h"
#include "ReadyForSaleState.h"
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
#include "MinimalPruningStrategy.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

PlantProduct::PlantProduct(const std::string &id, PlantSpeciesProfile *profile)
    : currentState(nullptr), daysInCurrentState(0), stateStartTime(std::chrono::steady_clock::now()),
      lastCareNotification(std::chrono::steady_clock::now()), monitor(nullptr), speciesProfile(profile), plantId(id)
{
    transitionTo(new PlantedState());
    addStrategy("water", new WateringStrategy());
    addStrategy("mist", new GentleMistStrategy());
    addStrategy("prune_artistic", new ArtisticPruningStrategy());
    addStrategy("fertilize", new FertilizingStrategy());
    addStrategy("flood", new FloodWateringStrategy());
    addStrategy("prune_standard", new StandardPruningStrategy());
    addStrategy("drip", new DripWateringStrategy());
    addStrategy("prune_minimal", new MinimalPruningStrategy());
}

PlantProduct::~PlantProduct()
{
    delete currentState;
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
    stateStartTime = std::chrono::steady_clock::now();
    lastCareNotification = std::chrono::steady_clock::now();
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
    std::string normalized = careType;
    std::transform(normalized.begin(), normalized.end(), normalized.begin(), [](unsigned char c)
                   { return static_cast<char>(std::tolower(c)); });

    // Dynamic pruning strategy selection
    if (normalized == "pruning" || normalized == "prune")
    {
        std::string idealPruning = speciesProfile->getProperty("idealPruning");
        if (idealPruning == "artistic")
        {
            normalized = "prune_artistic";
        }
        else if (idealPruning == "minimal")
        {
            normalized = "prune_minimal";
        }
        else
        {
            normalized = "prune_standard"; // Default
        }
    }
    else
    {
        // Legacy alias logic for other care types
        static const std::map<std::string, std::string> aliases = {
            {"watering", "water"},
            {"water", "water"},
            {"mist", "mist"},
            {"fertilizing", "fertilize"},
            {"fertilize", "fertilize"},
            {"flood", "flood"},
            {"drip", "drip"}};
        auto aliasIt = aliases.find(normalized);
        if (aliasIt != aliases.end())
        {
            normalized = aliasIt->second;
        }
    }

    auto it = strategy_map.find(normalized);
    if (it != strategy_map.end())
    {
        std::string amountStr;
        int amount = 1;          // Default amount for pruning, as it's not numeric
        std::string propertyKey; // Declare propertyKey here

        if (normalized.rfind("prune", 0) != 0)
        { // if not a pruning type
            if (normalized == "water" || normalized == "mist" || normalized == "flood" || normalized == "drip")
            {
                propertyKey = "idealWater";
            }
            else if (normalized == "fertilize")
            {
                propertyKey = "idealFertilizer";
            }
            amountStr = propertyKey.empty() ? std::string() : speciesProfile->getProperty(propertyKey);
            amount = amountStr.empty() ? 100 : std::stoi(amountStr);
        }

        std::cout << "Performing '" << normalized << "' care for " << speciesProfile->getSpeciesName() << "." << std::endl;
        it->second->applyCare(amount, normalized);
    }
    else
    {
        std::cout << "Warning: No strategy found for care type '" << careType << "' for " << speciesProfile->getSpeciesName() << " plants." << std::endl;
    }
}

void PlantProduct::advanceLifecycle()
{
    if (currentState)
    {
        daysInCurrentState++;
        currentState->advanceState(this);
    }
}

int PlantProduct::getSecondsInCurrentState() const
{
    auto now = std::chrono::steady_clock::now();
    return static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(now - stateStartTime).count());
}

int PlantProduct::getSecondsSinceLastCare() const
{
    auto now = std::chrono::steady_clock::now();
    return static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(now - lastCareNotification).count());
}

void PlantProduct::resetLastCareTime()
{
    lastCareNotification = std::chrono::steady_clock::now();
}

std::string PlantProduct::getStrategyNameForCareType(const std::string &careType) const
{
    std::string normalized = careType;
    std::transform(normalized.begin(), normalized.end(), normalized.begin(), [](unsigned char c)
                   { return static_cast<char>(std::tolower(c)); });

    if (normalized == "pruning" || normalized == "prune")
    {
        std::string idealPruning = speciesProfile->getProperty("idealPruning");
        if (idealPruning == "artistic")
        {
            normalized = "prune_artistic";
        }
        else if (idealPruning == "minimal")
        {
            normalized = "prune_minimal";
        }
        else
        {
            normalized = "prune_standard";
        }
    }
    else
    {
        static const std::map<std::string, std::string> aliases = {
            {"watering", "water"},
            {"water", "water"},
            {"mist", "mist"},
            {"fertilizing", "fertilize"},
            {"fertilize", "fertilize"},
            {"flood", "flood"},
            {"drip", "drip"}};
        auto aliasIt = aliases.find(normalized);
        if (aliasIt != aliases.end())
        {
            normalized = aliasIt->second;
        }
    }

    auto it = strategy_map.find(normalized);
    if (it != strategy_map.end())
    {
        return it->second->getName();
    }
    return "Unknown";
}
