#ifndef ORDERVALIDATIONHANDLER_H
#define ORDERVALIDATIONHANDLER_H

#include "OrderProcessHandler.h"
#include "InventoryManager.h"
#include "SinglePlant.h"
#include "PlantBundle.h"

/**
 * @brief Concrete handler for order validation
 * Validates that all items in the order are available in inventory
 */
class OrderValidationHandler : public OrderProcessHandler {
public:
    OrderValidationHandler() : OrderProcessHandler("Order Validation") {}
    
protected:
    bool processOrder(Order* order, Customer* customer) override {
        logStep("Checking inventory availability for all order items...");
        
        if (!order || order->isEmpty()) {
            std::cout << "[ERROR] Order is empty or invalid" << std::endl;
            return false;
        }
        
        InventoryManager& inventory = InventoryManager::getInstance();
        auto availablePlants = inventory.getReadyForSalePlants();
        
        logStep("Available plants in sales floor: " + std::to_string(availablePlants.size()));
        
        // Validate each order item
        for (const auto* orderItem : order->getOrderItems()) {
            if (!validateOrderItem(orderItem, availablePlants)) {
                return false;
            }
        }
        
        logStep("All items are available in inventory");
        order->setStatus("Validated");
        return true;
    }
    
private:
    bool validateOrderItem(const OrderItem* item, const std::vector<PlantProduct*>& availablePlants) {
        // Check if it's a SinglePlant
        if (const SinglePlant* singlePlant = dynamic_cast<const SinglePlant*>(item)) {
            return validateSinglePlant(singlePlant, availablePlants);
        }
        // Check if it's a PlantBundle
        else if (const PlantBundle* bundle = dynamic_cast<const PlantBundle*>(item)) {
            return validatePlantBundle(bundle, availablePlants);
        }
        
        logStep("Unknown order item type");
        return false;
    }
    
    bool validateSinglePlant(const SinglePlant* plant, const std::vector<PlantProduct*>& availablePlants) {
        std::string plantType = plant->getPlantType();
        int requiredQuantity = plant->getQuantity();
        int availableCount = 0;
        
        // Count available plants of this type
        for (const auto* availablePlant : availablePlants) {
            if (availablePlant->getProfile() && 
                availablePlant->getProfile()->getSpeciesName() == plantType) {
                availableCount++;
            }
        }
        
        if (availableCount >= requiredQuantity) {
            logStep("[OK] " + plantType + ": " + std::to_string(requiredQuantity) + 
                   " required, " + std::to_string(availableCount) + " available");
            return true;
        } else {
            std::cout << "[ERROR] Insufficient " << plantType << ": " << requiredQuantity 
                     << " required, only " << availableCount << " available" << std::endl;
            return false;
        }
    }
    
    bool validatePlantBundle(const PlantBundle* bundle, const std::vector<PlantProduct*>& availablePlants) {
        logStep("Validating bundle: " + bundle->getName());
        
        // Validate all items in the bundle
        for (const auto* bundleItem : bundle->getItems()) {
            if (!validateOrderItem(bundleItem, availablePlants)) {
                return false;
            }
        }
        
        logStep("[OK] Bundle validation completed");
        return true;
    }
};

#endif