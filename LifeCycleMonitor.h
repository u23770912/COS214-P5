#ifndef LIFECYCLE_MONITOR_H
#define LIFECYCLE_MONITOR_H

#include <vector>
#include <iostream>
#include "PlantProduct.h"


/**
 * @class LifeCycleMonitor
 * @brief Monitors the lifecycle of a PlantProduct and notifies registered observers of changes
 *
 * This class implements the Observer pattern to manage and notify observers
 * about lifecycle events of a PlantProduct subject.
 */

class LifeCycleMonitor
{
    private:
        // LIST OF OF REGISTERS OBSERVER'S STORE IN A VECTOR
        std::vector<LifeCycleObserver *> observers;

        // THE SPECIFIC PLANT PRODUCT BEING MONITORED
        PlantProduct *subject;

    public:
        LifeCycleMonitor(PlantProduct *plant);

        void registerObserver(LifeCycleObserver *observer);

        void removeObserver(LifeCycleObserver *observer);

        void notify(const std::string &commandType);
};

#endif
