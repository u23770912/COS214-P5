#ifndef STAFFMEMBER_H
#define STAFFMEMBER_H

#include "StaffChainHandler.h"
#include "Command.h"
#include "CareCommand.h"
#include <string>
#include <map>
#include <iostream>

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
