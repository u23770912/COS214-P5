#ifndef LIFECYCLE_MONITOR_H
#define LIFECYCLE_MONITOR_H

#include <vector>
#include "LifeCycleObserver.h"

class PlantProduct;

class LifeCycleMonitor {
private:
    std::vector<LifeCycleObserver*> observers;
    PlantProduct* subject;

public:
    LifeCycleMonitor(PlantProduct* plant);
    
    void registerObserver(LifeCycleObserver* observer);
    
    void removeObserver(LifeCycleObserver* observer);
    
    void notify();
};

#endif