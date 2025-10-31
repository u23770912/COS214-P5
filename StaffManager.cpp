#include "StaffManager.h"
#include "StaffMember.h"
#include "Customer.h"
#include "Order.h"
#include "Command.h"
#include "PlantProduct.h"
#include <iostream>

/**
 * @file StaffManager.cpp
 * @brief Implementation of StaffManager - Observer and Command Dispatcher
 * 
 * StaffManager serves two key roles:
 * 1. LifeCycleObserver: Monitors plant lifecycle events and creates commands
 * 2. CustomerObserver: Monitors customer interactions and validates orders
 * 
 * When a plant triggers a lifecycle event (e.g., transitions to ReadyForSale),
 * the StaffManager creates the appropriate command and dispatches it through
 * the staff chain via StaffMember.
 */

StaffManager::StaffManager(StaffMember* dispatcher) 
    : staffDispatcher(dispatcher) {}

StaffManager::~StaffManager() {
    // Note: StaffManager does not own the staffDispatcher
    // It's managed externally (typically in main or test files)
}

// ============================================================================
// LifeCycleObserver Implementation - Plant Lifecycle Event Handling
// ============================================================================

void StaffManager::update(PlantProduct* plant, const std::string& commandType) {
    if (!plant) {
        std::cout << "[STAFF MANAGER] Error: Received null plant reference." << std::endl;
        return;
    }
    
    std::cout << "[STAFF MANAGER] Plant lifecycle event detected" << std::endl;
    std::cout << "   Plant State: " << plant->getCurrentStateName() << std::endl;
    std::cout << "   Command Type: " << commandType << std::endl;
    
    // Create command using the Prototype pattern (Command factory)
    Command* command = Command::createCommand(commandType);
    
    if (command) {
        // Set the plant as the receiver of the command
        command->setReceiver(plant);
        
        std::cout << "[STAFF MANAGER] Command created successfully. Dispatching to staff chain..." << std::endl;
        
        // Dispatch the command through the staff chain
        dispatchCommand(command);
    } else {
        std::cout << "[STAFF MANAGER] Error: Failed to create command of type '" 
                  << commandType << "'." << std::endl;
        std::cout << "[STAFF MANAGER] Plant may be transitioning to withering state..." << std::endl;
        
        // If command creation fails, transition plant to withering
        plant->transitionToWithering();
    }
}

void StaffManager::dispatchCommand(Command* command) {
    if (!staffDispatcher) {
        std::cout << "[STAFF MANAGER] Error: No staff dispatcher available!" << std::endl;
        if (command) {
            std::cout << "[STAFF MANAGER] Command '" << command->getType() << "' dropped." << std::endl;
            delete command;
        }
        return;
    }
    
    if (!command) {
        std::cout << "[STAFF MANAGER] Error: Cannot dispatch null command." << std::endl;
        return;
    }
    
    std::cout << "[STAFF MANAGER] Dispatching command '" << command->getType() 
              << "' to staff chain..." << std::endl;
    std::cout << "   Required Role: " << command->getRequiredRole() << std::endl;
    
    // StaffMember will route the command to the appropriate team based on required role
    // The command will then travel through the chain of responsibility until handled
    staffDispatcher->dispatch(command);
}

// CustomerObserver implementation - for customer interaction events
void StaffManager::updateCustomerInteraction(Customer* customer, const std::string& interactionType, const std::string& details) {
    std::cout << "\n[STAFF MANAGER] Received customer interaction notification" << std::endl;
    std::cout << "   Customer: " << customer->getName() << std::endl;
    std::cout << "   Interaction: " << interactionType << std::endl;
    
    if (!details.empty()) {
        std::cout << "   Details: " << details << std::endl;
    }
    
    // Handle different types of customer interactions
    if (interactionType == "PlantSelection") {
        std::cout << "   [ACTION] Staff noting customer plant selection" << std::endl;
    } else if (interactionType == "BundleCreation") {
        std::cout << "   [ACTION] Staff assisting with bundle creation" << std::endl;
    } else if (interactionType == "AssistanceNeeded") {
        std::cout << "   [ACTION] Dispatching staff to assist customer" << std::endl;
    } else {
        std::cout << "   [ACTION] Staff monitoring customer activity" << std::endl;
    }
}

bool StaffManager::validateCustomerOrder(Order* order, Customer* customer) {
    std::cout << "\n[STAFF MANAGER] Validating customer order" << std::endl;
    std::cout << "   Customer: " << customer->getName() << " (" << customer->getEmail() << ")" << std::endl;
    std::cout << "   Order ID: " << order->getOrderId() << std::endl;
    std::cout << "   Total Items: " << order->getItemCount() << std::endl;
    std::cout << "   Total Amount: $" << order->getTotalAmount() << std::endl;
    
    // Basic validation checks
    if (order->isEmpty()) {
        std::cout << "   [VALIDATION FAILED] Order is empty" << std::endl;
        return false;
    }
    
    if (order->getTotalAmount() <= 0) {
        std::cout << "   [VALIDATION FAILED] Invalid order total" << std::endl;
        return false;
    }
    
    std::cout << "   [VALIDATION PASSED] Order approved by staff" << std::endl;
    return true;
}

