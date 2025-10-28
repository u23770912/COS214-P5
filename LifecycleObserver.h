#ifndef LIFECYCLE_OBSERVER_H
#define LIFECYCLE_OBSERVER_H
#include <iostream>
#include "PlantProduct.h"

class PlantProduct;

/**
 * @class LifeCycleObserver
 * @brief Abstract base class for observers that monitor PlantProduct lifecycle events
 *
 * This class defines the interface for objects that need to be notified
 * about changes in the lifecycle of a PlantProduct.
 */

class LifeCycleObserver
{
public:
    virtual ~LifeCycleObserver() {}

    virtual void update(PlantProduct *plant, const std::string &commandType) = 0;
};

#endif