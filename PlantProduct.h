#ifndef PLANT_PRODUCT_H
#define PLANT_PRODUCT_H

#include <vector>
#include <string>
#include <map>
#include "./ObserverDP/LifeCycleObserver.h"
#include "./StateDP/PlantState.h"
//#include "./BridgeDP/PlantSpeciesProfile.h"  // File doesn't exist yet
//#include "./StrategyDP/CareStrategy.h"      // File doesn't exist yet

// Forward declarations for classes that don't exist yet
class PlantSpeciesProfile;
class CareStrategy;

// The main context class that ties many patterns together.
class PlantProduct {
private:
    // State Pattern
    PlantState* currentState;
    // Observer Pattern
    LifecycleObserver* monitor;
    // Bridge Pattern
    PlantSpeciesProfile* speciesProfile;
    // Strategy Pattern
    std::map<std::string, CareStrategy*> strategy_map;
    std::string plantId;

public:
    PlantProduct(const std::string& id, PlantSpeciesProfile* profile);
    ~PlantProduct();

    // --- State ---
    void transitionTo(PlantState* state);
    std::string getCurrentStateName() const;
    void transitionToWithering();

    // --- Observer ---
    void setObserver(LifecycleObserver* obs) { monitor = obs; }

    // --- Bridge ---
    PlantSpeciesProfile* getProfile() const;

    // --- Strategy ---
    void addStrategy(const std::string& careType, CareStrategy* strategy);
    void performCare(const std::string& careType);

    // --- Business Logic ---
    void advanceLifecycle();
    void notify(const std::string& commandType);
};

#endif // PLANT_PRODUCT_H