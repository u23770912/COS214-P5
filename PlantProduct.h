#ifndef PLANT_PRODUCT_H
#define PLANT_PRODUCT_H

#include <vector>
#include <string>
#include <map>
#include <chrono>
#include "PlantState.h"
#include "PlantSpeciesProfile.h"
#include "CareStrategy.h"

class LifeCycleObserver;

// The main context class that ties many patterns together.
class PlantProduct
{
private:
    // State Pattern
    PlantState *currentState;
    int daysInCurrentState;
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
    int getDaysInCurrentState() const { return daysInCurrentState; };
    int getSecondsInCurrentState() const;
    int getSecondsSinceLastCare() const;
    void resetLastCareTime();

    bool isCareTypeAppropriate(const std::string &careType) const;

    // --- Observer ---
    void setObserver(LifeCycleObserver *obs) { monitor = obs; }

    // --- Bridge ---
    PlantSpeciesProfile *getProfile() const;

    // --- Strategy ---
    void addStrategy(const std::string &careType, CareStrategy *strategy);
    void performCare(const std::string &careType);

    const std::string &getId() const { return plantId; }

    // --- Business Logic ---
    void advanceLifecycle();
    void notify(const std::string &commandType);

    const std::map<std::string, CareStrategy*>& getStrategyMap() const { 
    return strategy_map; 
}
};

#endif // PLANT_PRODUCT_H
