#include "PlantProduct.h"
<<<<<<< HEAD
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
=======
#include "StateDP/PlantState.h"
#include "BridgeDP/PlantSpeciesProfile.h"
#include "StrategyDP/CareStrategy.h"
#include "StateDP/WitheringState.h"
#include <iostream>

PlantProduct::PlantProduct(const std::string& id, PlantSpeciesProfile* profile) 
    : currentState(nullptr), monitor(nullptr), speciesProfile(profile), plantId(id) {
    // Initial state could be set here, e.g., transitionTo(new PlantedState());
}

PlantProduct::~PlantProduct() {
    delete currentState;
    delete speciesProfile;
    for (auto& pair : strategy_map) {
>>>>>>> a5cc157f06bf6b3c50e503d2c0883ab9968a4990
        delete pair.second;
    }
}

<<<<<<< HEAD
void PlantProduct::transitionTo(PlantState *state)
{
    if (currentState)
    {
=======
void PlantProduct::transitionTo(PlantState* state) {
    if (currentState) {
>>>>>>> a5cc157f06bf6b3c50e503d2c0883ab9968a4990
        currentState->onExit(this);
        delete currentState;
    }
    currentState = state;
    currentState->onEnter(this);
<<<<<<< HEAD
    daysInCurrentState = 0; // Reset days when transitioning
}

std::string PlantProduct::getCurrentStateName() const
{
    if (currentState)
    {
=======
}

std::string PlantProduct::getCurrentStateName() const {
    if (currentState) {
>>>>>>> a5cc157f06bf6b3c50e503d2c0883ab9968a4990
        return currentState->getName();
    }
    return "None";
}

<<<<<<< HEAD
void PlantProduct::transitionToWithering()
{
=======
void PlantProduct::transitionToWithering() {
>>>>>>> a5cc157f06bf6b3c50e503d2c0883ab9968a4990
    std::cout << "Transitioning plant to withering state due to an error or neglect." << std::endl;
    transitionTo(new WitheringState());
}

<<<<<<< HEAD
void PlantProduct::notify(const std::string &commandType)
{
    if (monitor)
    {
=======
void PlantProduct::notify(const std::string& commandType) {
    if (monitor) {
>>>>>>> a5cc157f06bf6b3c50e503d2c0883ab9968a4990
        monitor->update(this, commandType);
    }
}

<<<<<<< HEAD
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
=======
PlantSpeciesProfile* PlantProduct::getProfile() const {
    return speciesProfile;
}

void PlantProduct::addStrategy(const std::string& careType, CareStrategy* strategy) {
    strategy_map[careType] = strategy;
}

void PlantProduct::performCare(const std::string& careType) {
    auto it = strategy_map.find(careType);
    if (it != strategy_map.end()) {
        std::cout << "Performing '" << careType << "' care for " << speciesProfile->getSpeciesName() << "." << std::endl;
        // Pass a relevant property from the profile to the strategy
        std::string property = speciesProfile->getProperty("some_property_for_" + careType);
        it->second->execute(property);
    } else {
>>>>>>> a5cc157f06bf6b3c50e503d2c0883ab9968a4990
        std::cout << "Warning: No strategy found for care type '" << careType << "'." << std::endl;
    }
}

<<<<<<< HEAD
void PlantProduct::advanceLifecycle()
{
    if (currentState)
    {
        // The concrete state's advanceState will handle the transition logic
        daysInCurrentState++;
=======
void PlantProduct::advanceLifecycle() {
    if (currentState) {
        // The concrete state's advanceState will handle the transition logic
>>>>>>> a5cc157f06bf6b3c50e503d2c0883ab9968a4990
        currentState->advanceState(this);
    }
}
