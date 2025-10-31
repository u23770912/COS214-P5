#include "StaffMember.h"
#include "Command.h"
#include "PlantProduct.h"
#include <iostream>

/**
 * @file StaffMember.cpp
 * @brief Implementation of StaffMember - Staff Team Coordinator
 * 
 * StaffMember acts as a dispatcher that routes commands to the appropriate
 * staff team based on the command's required role. Each team is organized
 * as a Chain of Responsibility.
 * 
 * Architecture:
 * - StaffMember maintains a map of role -> team chain head
 * - When a command arrives, it's routed to the appropriate team
 * - The team chain then processes the command through Chain of Responsibility
 * 
 * Example:
 *   Command "MoveToSalesFloor" requires role "Sales"
 *   -> StaffMember routes to Sales team
 *   -> Cashier1 -> Cashier2 -> ... (Chain of Responsibility)
 */

void StaffMember::queueUnhandledCommand(Command* command) {
    if (command) {
        std::cout << "[STAFF MEMBER] Queueing unhandled command '" << command->getType() 
                  << "' for later processing." << std::endl;
        unhandledCommands.push(command);
    } else {
        std::cout << "[STAFF MEMBER] Warning: Attempted to queue null command." << std::endl;
    }
}

void StaffMember::processUnhandledQueue() {
    if (unhandledCommands.empty()) {
        std::cout << "[STAFF MEMBER] No unhandled commands in queue." << std::endl;
        return;
    }
    
    std::cout << "[STAFF MEMBER] Processing unhandled command queue (" 
              << unhandledCommands.size() << " commands)..." << std::endl;
    
    Command* command = unhandledCommands.front();
    unhandledCommands.pop();
    
    std::cout << "[STAFF MEMBER] Re-dispatching command '" << command->getType() 
              << "' from the unhandled queue." << std::endl;
    
    dispatch(command);
}

void StaffMember::dispatch(Command* command) {
    if (!command) {
        std::cout << "[STAFF MEMBER] Error: Received a null command." << std::endl;
        return;
    }

    std::string role = command->getRequiredRole();
    std::cout << "[STAFF MEMBER] Routing command '" << command->getType() 
              << "' to '" << role << "' team..." << std::endl;
    
    // Look up the team responsible for this role
    std::map<std::string, StaffChainHandler*>::iterator it = teams.find(role);

    if (it != teams.end()) {
        // Team found - pass command to the head of the chain
        std::cout << "[STAFF MEMBER] Team found. Passing to chain of responsibility..." << std::endl;
        it->second->handleCommand(command);
    } else {
        // No team registered for this role - system error
        std::cout << "[STAFF MEMBER] CRITICAL ERROR: No team registered for role '" 
                  << role << "'." << std::endl;
        std::cout << "[STAFF MEMBER] Command '" << command->getType() << "' cannot be processed." << std::endl;
        
        // If the command has a plant receiver, transition it to withering
        // (represents neglected plant care due to lack of staff)
        PlantProduct* plant = command->getReceiver();
        if (plant) {
            std::cout << "[STAFF MEMBER] Plant will transition to withering due to lack of care." << std::endl;
            plant->transitionToWithering();
        }
        
        // Clean up the command
        delete command;
    }
}