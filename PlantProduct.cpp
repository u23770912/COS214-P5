#include "PlantProduct.h"
#include "LifecycleObserver.h"
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
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

PlantProduct::PlantProduct(const std::string &id, PlantSpeciesProfile *profile)
    : currentState(nullptr), daysInCurrentState(0),
      stateStartTime(std::chrono::steady_clock::now()),
      lastCareNotification(std::chrono::steady_clock::now()),
      monitor(nullptr), speciesProfile(profile), plantId(id)
{

    transitionTo(new PlantedState());

    // Get plant category for strategy selection
    std::string category = profile->getProperty("category");

    // Assign strategies based on plant category
    if (category == "Tree")
    {
        addStrategy("water", new DripWateringStrategy()); // Trees need drip watering
        addStrategy("drip", new DripWateringStrategy());  // Alias for consistency
        addStrategy("prune_artistic", new ArtisticPruningStrategy());
        addStrategy("prune_standard", new StandardPruningStrategy());
        addStrategy("fertilize", new FertilizingStrategy());
    }
    else if (category == "Flower")
    {
        addStrategy("water", new WateringStrategy());  // Regular watering
        addStrategy("mist", new GentleMistStrategy()); // Optional misting
        addStrategy("prune_standard", new StandardPruningStrategy());
        addStrategy("fertilize", new FertilizingStrategy());
        // Flowers don't need artistic pruning
    }
    else if (category == "Cactus" || category == "Succulent")
    {
        addStrategy("water", new DripWateringStrategy()); // Cacti need drip
        addStrategy("drip", new DripWateringStrategy());  // Alias
        addStrategy("fertilize", new FertilizingStrategy());
    }
    else
    {
        // Default strategies for unknown types
        addStrategy("water", new WateringStrategy());
        addStrategy("fertilize", new FertilizingStrategy());
    }
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

/*
void PlantProduct::performCare(const std::string &careType)
{
    std::string normalized = careType;
    std::transform(normalized.begin(), normalized.end(), normalized.begin(), [](unsigned char c)
                   { return static_cast<char>(std::tolower(c)); });

    static const std::map<std::string, std::string> aliases = {
        {"watering", "water"},
        {"water", "water"},
        {"mist", "mist"},
        {"fertilizing", "fertilize"},
        {"fertilize", "fertilize"},
        {"pruning", "prune_standard"},
        {"prune", "prune_standard"},
        {"prune_standard", "prune_standard"},
        {"prune_artistic", "prune_artistic"},
        {"flood", "flood"},
        {"drip", "drip"}};

    auto aliasIt = aliases.find(normalized);
    if (aliasIt != aliases.end())
    {
        normalized = aliasIt->second;
    }

    auto it = strategy_map.find(normalized);
    if (it != strategy_map.end())
    {
        std::string propertyKey;
        if (normalized == "water" || normalized == "mist" || normalized == "flood" || normalized == "drip")
        {
            propertyKey = "idealWater";
        }
        else if (normalized == "fertilize")
        {
            propertyKey = "idealFertilizer";
        }
        else if (normalized == "prune_artistic" || normalized == "prune_standard")
        {
            propertyKey = "idealPruning";
        }

        std::string amountStr = propertyKey.empty() ? std::string() : speciesProfile->getProperty(propertyKey);
        int amount = amountStr.empty() ? 100 : std::stoi(amountStr);
        std::cout << "Performing '" << normalized << "' care for " << speciesProfile->getSpeciesName() << "." << std::endl;
        it->second->applyCare(amount, normalized);
    }
    else
    {
        std::cout << "Warning: No strategy found for care type '" << careType << "'." << std::endl;
    }
}
*/

void PlantProduct::performCare(const std::string &careType)
{
    std::string normalized = careType;
    std::transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);

        std::cout << "DEBUG: Performing care '" << careType << "' -> normalized: '" << normalized 
              << "' for " << getProfile()->getSpeciesName() << std::endl;

    // Validate if this care type is appropriate
    if (!isCareTypeAppropriate(normalized))
    {
        return; // Skip inappropriate care
    }

    // Existing alias mapping
    static const std::map<std::string, std::string> aliases = {
        {"watering", "water"}, {"water", "water"}, {"mist", "mist"}, {"misting", "mist"}, {"fertilizing", "fertilize"}, {"fertilize", "fertilize"}, {"pruning", "prune_standard"}, {"prune", "prune_standard"}, {"prune_standard", "prune_standard"}, {"prune_artistic", "prune_artistic"}, {"flood", "flood"}, {"drip", "drip"}, {"drip_watering", "drip"}};

    auto aliasIt = aliases.find(normalized);
    if (aliasIt != aliases.end())
    {
        normalized = aliasIt->second;
    }

    auto it = strategy_map.find(normalized);
    if (it != strategy_map.end())
    {
        std::string propertyKey;
        if (normalized == "water" || normalized == "mist" || normalized == "flood" || normalized == "drip")
        {
            propertyKey = "idealWater";
        }
        else if (normalized == "fertilize")
        {
            propertyKey = "idealFertilizer";
        }
        else if (normalized.find("prune") != std::string::npos)
        {
            propertyKey = "idealPruning";
        }

        std::string amountStr = propertyKey.empty() ? std::string() : speciesProfile->getProperty(propertyKey);
        int amount = 100; // Default amount

        if (!amountStr.empty())
        {
            try
            {
                // Extract only digits from strings like "250ml"
                std::string numericPart;
                for (size_t i = 0; i < amountStr.length(); ++i)
                {
                    if (std::isdigit(amountStr[i]))
                    {
                        numericPart += amountStr[i];
                    }
                    else if (!numericPart.empty())
                    {
                        // Stop at first non-digit after we found some digits
                        break;
                    }
                }
                if (!numericPart.empty())
                {
                    amount = std::stoi(numericPart);
                }
            }
            catch (const std::exception &e)
            {
                std::cout << "Warning: Could not parse amount from '" << amountStr
                          << "', using default: " << amount << std::endl;
            }
        }

        std::cout << "Performing '" << normalized << "' care for "
                  << speciesProfile->getSpeciesName() << " ("
                  << speciesProfile->getProperty("category") << ")." << std::endl;
        it->second->applyCare(amount, normalized);
    }
    else
    {
        std::cout << "Warning: No strategy found for care type '" << careType
                  << "' for " << speciesProfile->getProperty("category") << " plants." << std::endl;
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

bool PlantProduct::isCareTypeAppropriate(const std::string &careType) const
{
    std::string category = speciesProfile->getProperty("category");

    if (category == "Cactus" || category == "Succulent")
    {
        // Cacti shouldn't be misted or flood-watered - causes rot
        if (careType == "mist" || careType == "flood")
        {
            std::cout << "WARNING: " << careType << " can harm " << category << " plants!" << std::endl;
            return false;
        }
        // Cacti don't need pruning
        if (careType.find("prune") != std::string::npos)
        {
            std::cout << "NOTE: " << category << " plants rarely need pruning." << std::endl;
        }
    }
    else if (category == "Tree")
    {
        // Trees don't need misting
        if (careType == "mist")
        {
            std::cout << "NOTE: Mist watering is not effective for " << category << " plants." << std::endl;
            return false;
        }
    }
    else if (category == "Flower")
    {
        // Flowers don't need artistic pruning
        if (careType == "prune_artistic")
        {
            std::cout << "NOTE: Artistic pruning is not typical for " << category << " plants." << std::endl;
        }
    }

    return true;
}
