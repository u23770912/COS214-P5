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

// The main context class that ties many patterns together.
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
    LifeCycleObserver *monitor;
    // Bridge Pattern
    PlantSpeciesProfile *speciesProfile;
    // Strategy Pattern
    std::map<std::string, CareStrategy *> strategy_map;
    std::string plantId;

public:
    PlantProduct(const std::string &id, PlantSpeciesProfile *profile);
    ~PlantProduct();

    // --- State ---
    void transitionTo(PlantState *state);
    std::string getCurrentStateName() const;
    void transitionToWithering();
    int getDaysInCurrentState() const { return daysInCurrentState; }

    // --- Timing ---
    int getSecondsInCurrentState() const;
    int getSecondsSinceLastCare() const;
    void resetLastCareTime();

    // --- Observer ---
    void setObserver(LifeCycleObserver *obs) { monitor = obs; }

    // --- Bridge ---
    PlantSpeciesProfile *getProfile() const;

    // --- Strategy ---
    void addStrategy(const std::string &careType, CareStrategy *strategy);
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