#include "LifeCycleMonitor.h"
#include "PlantProduct.h"

LifeCycleMonitor::LifeCycleMonitor(PlantProduct *plant) : subject(plant) {}

void LifeCycleMonitor::registerObserver(LifeCycleObserver *observer)
{
    if (observer != nullptr)
    {
        observers.push_back(observer);
    }
}

void LifeCycleMonitor::removeObserver(LifeCycleObserver *observer)
{
    for (auto it = observers.begin(); it != observers.end(); ++it)
    {
        if (*it == observer)
        {
            observers.erase(it);
            break;
        }
    }
}

void LifeCycleMonitor::notify(const std::string &commandTyp)
{
    for (LifeCycleObserver *observer : observers)
    {
        if (observer != nullptr)
        {
            observer->update(subject, commandTyp);
        }
    }
}