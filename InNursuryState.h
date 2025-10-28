#ifndef INNURSERYSTATE_H
#define INNURSERYSTATE_H

#include "PlantState.h"

class LifeCycleMonitor;

class InNurseryState : public PlantState {
public:
    InNurseryState();
    InNurseryState(PlantProduct* plant);
    virtual ~InNurseryState();
    
    // Override virtual methods from PlantState
    virtual void advanceState(PlantProduct* plant) ;
    virtual void onEnter(PlantProduct* plant) ;
    virtual void onExit(PlantProduct* plant) ;
    
    virtual std::string Name()  ;
    virtual LifeCycleMonitor* getLifeCycleMonitor()  ;
};

#endif 