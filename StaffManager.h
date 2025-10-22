#ifndef STAFF_MANAGER_H
#define STAFF_MANAGER_H

#include <string>
#include "LifeCycleObserver.h"
#include "StaffMember.h"

class CareCommand;

class StaffManager : public LifeCycleObserver {
private:
    StaffMember* greenhouseChainHead;
    StaffMember* salesChainHead;

public:
    StaffManager();
    
    ~StaffManager();
    
    void update(PlantProduct* plant) override;

    void dispatchToChain(CareCommand* command, const std::string& role);
};

#endif