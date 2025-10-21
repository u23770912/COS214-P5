#ifndef PLANT_PRODUCT_H
#define PLANT_PRODUCT_H

#include <vector>
#include <string>
#include <map>
#include "LifecycleObserver.h"
#include "PlantState.h"
#include "PlantSpeciesProfile.h"
#include "CareStrategy.h"

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

public:
    PlantProduct(PlantSpeciesProfile* profile);
    ~PlantProduct();

    // --- State ---
    void transitionTo(PlantState* state);
    std::string getCurrentStateName() const;
    void transitionToWithering();

    // --- Observer ---
    void notify(const std::string& commandType);

    // --- Bridge ---
    PlantSpeciesProfile* getProfile() const;

    // --- Strategy ---
    void addStrategy(const std::string& careType, CareStrategy* strategy);
    void performCare(const std::string& careType);

    // --- Business Logic ---
    void advanceLifecycle();
};

#endif // PLANT_PRODUCT_H
