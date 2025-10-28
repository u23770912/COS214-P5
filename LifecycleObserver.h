#ifndef LIFECYCLE_OBSERVER_H
#define LIFECYCLE_OBSERVER_H
#include <iostream>

// Forward declaration to avoid circular dependency
class PlantProduct;

// Observer Interface (Observer Pattern)
class LifeCycleObserver {
public:
    virtual ~LifeCycleObserver() {}
    virtual void update(PlantProduct* plant, const std::string& commandType) = 0;
};

#endif
