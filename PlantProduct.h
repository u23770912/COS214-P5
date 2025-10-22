#ifndef PLANT_PRODUCT_H
#define PLANT_PRODUCT_H

#include <string>
#include "PlantState.h"
#include "LifeCycleMonitor.h"

class PlantProduct {
private:
    std::string name;

    PlantState currentState;

    LifeCycleMonitor* monitor;


public:
    PlantProduct(const std::string& name);

    ~PlantProduct();
    
    void setState(PlantState newState);

    void advanceState();

    PlantState getCurrentState() const;

    void dispose();

    std::string getName() const;

    void registerObserver(LifeCycleObserver* observer);

    void removeObserver(LifeCycleObserver* observer);
};

#endif