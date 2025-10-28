#ifndef STAFFMANAGER_H
#define STAFFMANAGER_H

#include "LifecycleObserver.h"
#include "ModeVisitor.h"
#include <string>
#include <iostream>

class StaffMember;
class PlantProduct;
class Command;

/**
 * @class StaffManager
 * @brief Observes plants and delegates behavior to a ModeVisitor.
 */
class StaffManager : public LifeCycleObserver
{
private:
    StaffMember *staffDispatcher;
    ModeVisitor *currentModeVisitor;

public:
    StaffManager(StaffMember *dispatcher);
    ~StaffManager();

    void setMode(ModeVisitor *newMode);
    void update(PlantProduct *plant, const std::string &commandType) override;
    void dispatchCommand(Command *command);
};

#endif // STAFFMANAGER_H