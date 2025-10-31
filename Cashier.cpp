#include "Cashier.h"
#include "Command.h"
#include "MoveToSalesFloorCommand.h"
#include "Customer.h"
#include "OrderItem.h"
#include "SinglePlant.h"
#include "PlantBundle.h"
#include "InventoryManager.h"
#include <iostream>

void Cashier::handleCommand(Command* command) {
    if (!command) {
        std::cout << "[CASHIER] Error: Received null command." << std::endl;
        return;
    }
    
    // Check if this cashier is available to handle the command
    if (!isBusy()) {
        std::cout << "[CASHIER] Cashier is handling the '" << command->getType() << "' command." << std::endl;
        
        // Mark as busy and set active assignment
        activePlant = command->getReceiver();
        activeTask = command->getType();
        setBusy(true);
        
        // Handle different command types
        if (command->getType() == "MoveToSalesFloor") {
            processMoveToSalesFloor(command);
        } else {
            // Execute other commands directly
            command->execute();
        }
        
        // Command completed, mark as available
        setBusy(false);
        clearAssignment();
        delete command;
    } 
    // If busy, pass to next cashier in chain
    else if (next != nullptr) {
        std::cout << "[CASHIER] Cashier is busy, passing '" << command->getType() 
                  << "' to next in the sales team." << std::endl;
        next->handleCommand(command);
    } 
    // No one available in the entire chain
    else {
        std::cout << "[CASHIER] All Cashiers are busy!" << std::endl;
        
        // Try to queue the command if manager is available
        if (manager) {
            std::cout << "[CASHIER] Queueing the '" << command->getType() << "' task for later." << std::endl;
            manager->queueUnhandledCommand(command);
        } else {
            // No manager to queue with - handle based on command type
            std::cout << "[CASHIER] Error: No manager available to queue command." << std::endl;
            
            // Check if command has a plant receiver (care commands that affect plant health)
            if (command->getReceiver()) {
                // Only wither if it's not a MoveToSalesFloorCommand
                MoveToSalesFloorCommand* moveCmd = dynamic_cast<MoveToSalesFloorCommand*>(command);
                if (moveCmd == 0) {  // C++98 compatible null check
                    std::cout << "[CASHIER] Plant will wither due to neglected care." << std::endl;
                    command->getReceiver()->transitionToWithering();
                } else {
                    std::cout << "[CASHIER] Warning: Plant could not be moved to sales floor - will retry later." << std::endl;
                }
            }
            
            delete command;
        }
    }
}

void Cashier::processMoveToSalesFloor(Command* command) {
    std::cout << "[CASHIER] Processing MoveToSalesFloor request..." << std::endl;
    
    PlantProduct* plant = command->getReceiver();
    if (!plant) {
        std::cout << "[CASHIER] Error: No plant specified for MoveToSalesFloor command." << std::endl;
        return;
    }
    
    // Verify plant is in ReadyForSale state
    if (plant->getCurrentStateName() != "ReadyForSale") {
        std::cout << "[CASHIER] Error: Plant is not in ReadyForSale state (Current: " 
                  << plant->getCurrentStateName() << ")." << std::endl;
        std::cout << "[CASHIER] Cannot move to sales floor." << std::endl;
        return;
    }
    
    std::cout << "[CASHIER] Plant is ready for sale. Processing move to sales floor..." << std::endl;
    
    // Execute the command (which handles inventory operations)
    command->execute();
    
    std::cout << "[CASHIER] Successfully processed MoveToSalesFloor command!" << std::endl;
}

bool Cashier::validateOrder(Order* order, Customer* customer) {
    if (!order || !customer) {
        std::cout << "[CASHIER] Error: Invalid order or customer reference." << std::endl;
        return false;
    }
    
    std::cout << "\n[CASHIER] ===== Order Validation Start =====" << std::endl;
    std::cout << "[CASHIER] Customer: " << customer->getName() << std::endl;
    std::cout << "[CASHIER] Order ID: " << order->getOrderId() << std::endl;
    std::cout << "[CASHIER] Total Items: " << order->getItemCount() << std::endl;
    
    // Check if order is empty
    if (order->isEmpty()) {
        std::cout << "[CASHIER] Validation Failed: Order is empty." << std::endl;
        return false;
    }
    
    // Get available plants from inventory
    InventoryManager& inventory = InventoryManager::getInstance();
    std::vector<PlantProduct*> availablePlants = inventory.getReadyForSalePlants();
    
    std::cout << "[CASHIER] Checking inventory availability..." << std::endl;
    std::cout << "[CASHIER] Available plants in stock: " << availablePlants.size() << std::endl;
    
    if (availablePlants.empty()) {
        std::cout << "[CASHIER] Validation Failed: No plants available in inventory." << std::endl;
        return false;
    }
    
    // Validate each order item
    std::vector<OrderItem*> orderItems = order->getOrderItems();
    for (size_t i = 0; i < orderItems.size(); ++i) {
        OrderItem* item = orderItems[i];
        std::cout << "[CASHIER] Validating item " << (i+1) << "/" << orderItems.size() 
                  << ": " << item->getName() << std::endl;
        
        // Check if it's a SinglePlant
        SinglePlant* singlePlant = dynamic_cast<SinglePlant*>(item);
        if (singlePlant) {
            // Count matching plants in inventory
            int availableCount = 0;
            for (size_t j = 0; j < availablePlants.size(); ++j) {
                if (availablePlants[j]->getProfile()->getSpeciesName() == singlePlant->getPlantType()) {
                    ++availableCount;
                }
            }
            
            if (availableCount < singlePlant->getQuantity()) {
                std::cout << "[CASHIER] Validation Failed: Insufficient stock for " 
                          << singlePlant->getPlantType() << std::endl;
                std::cout << "[CASHIER]   Requested: " << singlePlant->getQuantity() 
                          << ", Available: " << availableCount << std::endl;
                return false;
            }
            
            std::cout << "[CASHIER]   OK: " << singlePlant->getPlantType() 
                      << " (Requested: " << singlePlant->getQuantity() 
                      << ", Available: " << availableCount << ")" << std::endl;
        }
        
        // Check if it's a PlantBundle
        PlantBundle* bundle = dynamic_cast<PlantBundle*>(item);
        if (bundle) {
            std::cout << "[CASHIER]   Validating bundle: " << bundle->getName() << std::endl;
            std::vector<OrderItem*> bundleItems = bundle->getItems();
            
            for (size_t j = 0; j < bundleItems.size(); ++j) {
                SinglePlant* bundlePlant = dynamic_cast<SinglePlant*>(bundleItems[j]);
                if (bundlePlant) {
                    int availableCount = 0;
                    for (size_t k = 0; k < availablePlants.size(); ++k) {
                        if (availablePlants[k]->getProfile()->getSpeciesName() == bundlePlant->getPlantType()) {
                            ++availableCount;
                        }
                    }
                    
                    if (availableCount < bundlePlant->getQuantity()) {
                        std::cout << "[CASHIER] Validation Failed: Insufficient stock in bundle for " 
                                  << bundlePlant->getPlantType() << std::endl;
                        std::cout << "[CASHIER]   Requested: " << bundlePlant->getQuantity() 
                                  << ", Available: " << availableCount << std::endl;
                        return false;
                    }
                    
                    std::cout << "[CASHIER]     Bundle item OK: " << bundlePlant->getPlantType() 
                              << " (Requested: " << bundlePlant->getQuantity() 
                              << ", Available: " << availableCount << ")" << std::endl;
                }
            }
        }
    }
    
    std::cout << "[CASHIER] Validation Passed: All items available in inventory!" << std::endl;
    std::cout << "[CASHIER] ===== Order Validation End =====" << std::endl;
    return true;
}
