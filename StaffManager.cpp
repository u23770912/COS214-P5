#include "StaffManager.h"
#include "StaffMember.h"
#include "Customer.h"
#include "Order.h"
#include "Command.h"
#include "PlantProduct.h"
#include <iostream>



StaffManager::StaffManager(StaffMember* dispatcher) 
    : staffDispatcher(dispatcher) {}

StaffManager::~StaffManager() {
}

// LifeCycleObserver implementation - for plant lifecycle events
void StaffManager::update(PlantProduct* plant, const std::string& commandType) {
    //currentModeVisitor->processUpdate(this, plant, commandType);
}

// CustomerObserver implementation - for customer interaction events
void StaffManager::updateCustomerInteraction(Customer* customer, 
                                            const std::string& interactionType, 
                                            const std::string& details) {
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
    
    Command* command = Command::createCommand(commandType);
    if (command) {
        command->setReceiver(plant);
        dispatchCommand(command);
    }
}

void StaffManager::dispatchCommand(Command* command) {
    if (staffDispatcher && command) {
        staffDispatcher->dispatch(command);
    }
}