#ifndef PLANT_PRODUCT_H
#define PLANT_PRODUCT_H

#include <vector>
#include <string>
#include <map>
#include <chrono>
#include "LifeCycleObserver.h"
#include "PlantState.h"
#include "PlantSpeciesProfile.h"
#include "CareStrategy.h"
#include "LifeCycleObserver.h"

class LifeCycleObserver;

/**
 * @brief Main plant product class that integrates multiple design patterns
 * 
 * This class serves as the context for multiple design patterns:
 * - State Pattern: Manages plant lifecycle states
 * - Observer Pattern: Notifies observers of plant state changes
 * - Bridge Pattern: Abstracts plant species characteristics
 * - Strategy Pattern: Implements different care strategies
 * 
 * Each plant has a unique ID, species profile, and can transition through
 * various states during its lifecycle while receiving different types of care.
 */
class PlantProduct
{
private:
    // State Pattern
    PlantState *currentState;
    int daysInCurrentState;

    // Timing for autonomous growth
    std::chrono::steady_clock::time_point stateStartTime;
    std::chrono::steady_clock::time_point lastCareNotification;

    // Observer Pattern
    LifeCycleObserver* monitor;
    // Bridge Pattern
    PlantSpeciesProfile *speciesProfile;
    // Strategy Pattern
    std::map<std::string, CareStrategy *> strategy_map;
    std::string plantId;

public:
    /**
     * @brief Construct a new Plant Product
     * @param id Unique identifier for the plant
     * @param profile Species profile containing plant characteristics
     */
    PlantProduct(const std::string &id, PlantSpeciesProfile *profile);
    
    /**
     * @brief Destructor
     */
    ~PlantProduct();

    // --- State Pattern Methods ---
    /**
     * @brief Transition plant to a new state
     * @param state New state to transition to
     */
    void transitionTo(PlantState *state);

    /**
     * @brief Get the name of the current state
     * @return String representing current state
     */
    std::string getCurrentStateName() const;

    /**
     * @brief Force transition to withering state
     */
    void transitionToWithering();

    /**
     * @brief Get days spent in current state
     * @return Number of days in current state
     */
    int getDaysInCurrentState() const { return daysInCurrentState; }

    // --- Timing Methods ---
    /**
     * @brief Get seconds spent in current state
     * @return Number of seconds in current state
     */
    int getSecondsInCurrentState() const;

    /**
     * @brief Get seconds since last care action
     * @return Number of seconds since last care
     */
    int getSecondsSinceLastCare() const;

    /**
     * @brief Reset the last care timestamp
     */
    void resetLastCareTime();

    // --- Observer Pattern Methods ---
    /**
     * @brief Set the lifecycle observer
     * @param obs Observer to notify of lifecycle events
     */
    void setObserver(LifeCycleObserver* obs) { monitor = obs; }

    // --- Bridge Pattern Methods ---
    /**
     * @brief Get the plant's species profile
     * @return Pointer to the plant's species profile
     */
    PlantSpeciesProfile *getProfile() const;

    // --- Strategy Pattern Methods ---
    /**
     * @brief Add a care strategy
     * @param careType Type of care (e.g., "Watering", "Fertilizing")
     * @param strategy Strategy implementation for the care type
     */
    void addStrategy(const std::string &careType, CareStrategy *strategy);

    /**
     * @brief Perform a care action
     * @param careType Type of care to perform
     */
    void performCare(const std::string &careType);

    // Get strategy name for a care type (for display purposes)
    std::string getStrategyNameForCareType(const std::string &careType) const;

    // --- Business Logic ---
    void advanceLifecycle();
    void notify(const std::string &commandType);

    // --- Plant ID ---
    std::string getId() const { return plantId; }

private:
    // Helper method to validate care appropriateness
    bool isCareTypeAppropriate(const std::string &careType) const;
};

#endif // PLANT_PRODUCT_H