#ifndef PLANT_PRODUCT_H
#define PLANT_PRODUCT_H

#include <vector>
#include <string>
#include <map>
#include "LifecycleObserver.h"
<<<<<<< HEAD
#include "PlantState.h"
#include "PlantSpeciesProfile.h"
#include "CareStrategy.h"
=======
#include "StateDP/PlantState.h"
#include "BridgeDP/PlantSpeciesProfile.h"
#include "StrategyDP/CareStrategy.h"
>>>>>>> a5cc157f06bf6b3c50e503d2c0883ab9968a4990

// The main context class that ties many patterns together.
class PlantProduct {
private:
    // State Pattern
    PlantState* currentState;
<<<<<<< HEAD
    int daysInCurrentState;
   
=======
>>>>>>> a5cc157f06bf6b3c50e503d2c0883ab9968a4990
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
<<<<<<< HEAD
    int getDaysInCurrentState() const { return daysInCurrentState; };
=======
>>>>>>> a5cc157f06bf6b3c50e503d2c0883ab9968a4990

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
