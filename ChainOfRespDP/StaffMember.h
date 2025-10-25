#ifndef STAFFMEMBER_H
#define STAFFMEMBER_H

#include "StaffChainHandler.h"
#include "../CommandDP/Command.h"
// #include "../CommandDP/CareCommand.h"  // File doesn't exist yet - commented out
#include <string>
#include <map>
#include <iostream>

// Forward declarations for classes that don't exist yet
class CareCommand;

class StaffMember {
private:
    // A map where the key is the role and the value is the team handler (not individual chain members).
    std::map<std::string, StaffChainHandler*> teams;

public:
    StaffMember() {}
    ~StaffMember() {
        // In a real application, manage memory of team handlers here.
    }

    void registerTeam(const std::string& role, StaffChainHandler* teamHandler) {
        teams[role] = teamHandler;
    }

    void dispatch(Command* command);
};

#endif // STAFFMEMBER_H