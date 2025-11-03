#ifndef ORDERVALIDATIONHANDLER_H
#define ORDERVALIDATIONHANDLER_H

#include "OrderProcessHandler.h"
#include "InventoryManager.h"
#include "SinglePlant.h"
#include "PlantBundle.h"
#include <vector>
#include <string>

/**
 * @brief Concrete handler for order validation
 * Validates that all items in the order are available in inventory
 */
class OrderValidationHandler : public OrderProcessHandler {
private:
    std::vector<std::string> validationErrors;
    
public:
    OrderValidationHandler() : OrderProcessHandler("Order Validation") {}
    
    std::vector<std::string> getValidationErrors() const {
        return validationErrors;
    }
    
protected:
    bool processOrder(Order* order, Customer* customer) override {
        validationErrors.clear(); // Clear previous errors
        
        logStep("Checking inventory availability for all order items...");
        
        if (!order || order->isEmpty()) {
            std::string error = "Order is empty or invalid. Please add items to your order before submitting.";
            validationErrors.push_back(error);
            std::cout << "\n[VALIDATION ERROR] " << error << std::endl;
            return false;
        }
        
        InventoryManager& inventory = InventoryManager::getInstance();
        auto availablePlants = inventory.getReadyForSalePlants();
        
        logStep("Available plants in sales floor: " + std::to_string(availablePlants.size()));
        
        if (availablePlants.empty()) {
            std::string error = "No plants are currently available on the sales floor. Please check back later or contact staff for assistance.";
            validationErrors.push_back(error);
            std::cout << "\n[VALIDATION ERROR] " << error << std::endl;
            return false;
        }
        
        // Validate each order item
        bool allValid = true;
        for (const auto* orderItem : order->getOrderItems()) {
            if (!validateOrderItem(orderItem, availablePlants)) {
                allValid = false;
            }
        }
        
        if (!allValid) {
            std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
            std::cout << "║     VALIDATION FAILED                 ║" << std::endl;
            std::cout << "╚════════════════════════════════════════╝" << std::endl;
            std::cout << "\nThe following issues were found with your order:\n" << std::endl;
            for (size_t i = 0; i < validationErrors.size(); i++) {
                std::cout << (i+1) << ". " << validationErrors[i] << std::endl;
            }
            std::cout << "\nPlease modify your order and try again." << std::endl;
            return false;
        }
        
        logStep("✓ All items are available in inventory");
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
        
        std::string error = "Unknown order item type encountered.";
        validationErrors.push_back(error);
        logStep("ERROR: " + error);
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
            logStep("✓ " + plantType + ": " + std::to_string(requiredQuantity) + 
                   " required, " + std::to_string(availableCount) + " available");
            return true;
        } else {
            std::string error;
            if (availableCount == 0) {
                error = "'" + plantType + "' is currently out of stock. We don't have any available at the moment.";
            } else {
                error = "Insufficient '" + plantType + "' plants available. You requested " + 
                       std::to_string(requiredQuantity) + " but we only have " + 
                       std::to_string(availableCount) + " in stock.";
            }
            validationErrors.push_back(error);
            std::cout << "  [✗] " << error << std::endl;
            return false;
        }
    }
    
    bool validatePlantBundle(const PlantBundle* bundle, const std::vector<PlantProduct*>& availablePlants) {
        logStep("Validating bundle: " + bundle->getName());
        
        bool bundleValid = true;
        // Validate all items in the bundle
        for (const auto* bundleItem : bundle->getItems()) {
            if (!validateOrderItem(bundleItem, availablePlants)) {
                bundleValid = false;
            }
        }
        
        if (bundleValid) {
            logStep("✓ Bundle '" + bundle->getName() + "' validation completed");
        } else {
            std::string error = "Bundle '" + bundle->getName() + "' contains items that are not available.";
            // Don't add to validationErrors as individual items already added their errors
            logStep("✗ " + error);
        }
        
        return bundleValid;
    }
};

#endif