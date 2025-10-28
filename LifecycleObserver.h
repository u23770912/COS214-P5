#ifndef LIFECYCLE_OBSERVER_H
#define LIFECYCLE_OBSERVER_H
#include <string>

class PlantProduct;

class LifeCycleObserver
{
public:
    virtual ~LifeCycleObserver() {}

    virtual void update(PlantProduct *plant, const std::string &commandType) = 0;
};

#endif