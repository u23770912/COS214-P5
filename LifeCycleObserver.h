#ifndef LIFECYCLE_OBSERVER_H
#define LIFECYCLE_OBSERVER_H

class PlantProduct;

class LifeCycleObserver {
public:
    virtual ~LifeCycleObserver() = default;
    
    virtual void update(PlantProduct* plant) = 0;
    
};

#endif