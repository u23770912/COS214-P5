#ifndef STAFFMEMBER_H
#define STAFFMEMBER_H

#include "StaffChainHandler.h"
#include "CareCommand.h"
#include <string>
#include <map>
#include <iostream>

/**
 * @class StaffMember
 * @brief A concrete dispatcher class that acts as a client to the staff team handlers.
 * 
 * This class is NOT part of the Chain of Responsibility itself. Instead, it owns
 * references to different team handlers (like GreenhouseStaff, SalesFloorStaff) and 
 * is responsible for routing commands to the correct team handler based on role.
 */
class StaffMember {
private:
    // A map where the key is the role and the value is the team handler (not individual chain members).
    std::map<std::string, StaffChainHandler*> teams;

public:
    StaffMember() {}
    ~StaffMember() {
        // In a real application, manage memory of team handlers here.
    }

    /**
     * @brief Registers a team handler with the dispatcher.
     * @param role The role name (e.g., "Greenhouse").
     * @param teamHandler A pointer to the team's handler object.
     */
    void registerTeam(const std::string& role, StaffChainHandler* teamHandler) {
        teams[role] = teamHandler;
    }

    /**
     * @brief Dispatches a command to the appropriate team handler.
     * @param command The command to be executed.
     */
    void dispatch(CareCommand* command) {
        if (!command) return;

        std::string role = command->getRequiredRole();
        auto it = teams.find(role);

        if (it != teams.end()) {
            // Found the team handler, delegate to it.
            it->second->handleCommand(command);
        } else {
            std::cout << "System Error: No team registered for role '" << role << "'. Plant will wither." << std::endl;
            command->getReceiver()->transitionToWithering();
            delete command;
        }
    }
};

#endif // STAFFMEMBER_H
