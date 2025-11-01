#include "Cashier.h"
#include "Command.h"
#include "MoveToSalesFloorCommand.h"
#include "Customer.h"
#include "OrderItem.h"
#include "SinglePlant.h"
#include "PlantBundle.h"
#include "InventoryManager.h"
#include <iostream>
#include <thread>
#include <chrono>

void Cashier::handleCommand(Command* command) {
    if (!command) {
        return;
    }
    
    // Check if this cashier is available to handle the command
    if (!isBusy()) {
        // Mark as busy and set active assignment
        activePlant = command->getReceiver();
        activeTask = command->getType();
        
        // Handle different command types
        if (command->getType() == "MoveToSalesFloor") {
            // Use threaded busy simulation (2 seconds) - busy flag auto-cleared by setBusyFor
            setBusyFor(std::chrono::seconds(2));
            processMoveToSalesFloor(command);
        } else {
            // Execute other commands directly with manual busy management
            setBusy(true);
            command->execute();
            setBusy(false);
            clearAssignment();
        }
        
        delete command;
    } 
    // If busy, pass to next cashier in chain
    else if (next != nullptr) {
        next->handleCommand(command);
    } 
    // No one available in the entire chain
    else {
        // Try to queue the command if manager is available
        if (manager) {
            manager->queueUnhandledCommand(command);
        } else {
            // No manager to queue with - handle based on command type
            // Check if command has a plant receiver (care commands that affect plant health)
            if (command->getReceiver()) {
                // Only wither if it's not a MoveToSalesFloorCommand
                MoveToSalesFloorCommand* moveCmd = dynamic_cast<MoveToSalesFloorCommand*>(command);
                if (moveCmd == 0) {  // C++98 compatible null check
                    command->getReceiver()->transitionToWithering();
                }
            }
            
            delete command;
        }
    }
}

void Cashier::processMoveToSalesFloor(Command* command) {
    PlantProduct* plant = command->getReceiver();
    if (!plant) {
        return;
    }
    
    // Verify plant is in ReadyForSale state
    if (plant->getCurrentStateName() != "ReadyForSale") {
        return;
    }
    
    // Execute the command immediately (busy duration handled by setBusyFor in handleCommand)
    command->execute();
}

bool Cashier::validateOrder(Order* order, Customer* customer) {
    if (!order || !customer) {
        return false;
    }
    
    // Check if order is empty
    if (order->isEmpty()) {
        return false;
    }
    
    // Get available plants from inventory
    InventoryManager& inventory = InventoryManager::getInstance();
    std::vector<PlantProduct*> availablePlants = inventory.getReadyForSalePlants();
    
    if (availablePlants.empty()) {
        return false;
    }
    
    // Validate each order item
    std::vector<OrderItem*> orderItems = order->getOrderItems();
    for (size_t i = 0; i < orderItems.size(); ++i) {
        OrderItem* item = orderItems[i];
        
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
                return false;
            }
        }
        
        // Check if it's a PlantBundle
        PlantBundle* bundle = dynamic_cast<PlantBundle*>(item);
        if (bundle) {
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
                        return false;
                    }
                }
            }
        }
    }
    
    return true;
}
