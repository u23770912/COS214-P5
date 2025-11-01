#ifndef PAYMENTPROCESSHANDLER_H
#define PAYMENTPROCESSHANDLER_H

#include "OrderProcessHandler.h"
#include "InventoryManager.h"
#include "SinglePlant.h"
#include "PlantBundle.h"
#include <random>
#include <iostream>
#include <map>

/**
 * @brief Concrete handler for payment processing
 * Simulates payment processing for the order
 */
class PaymentProcessHandler : public OrderProcessHandler {
private:
    std::mt19937 rng;
    
public:
    PaymentProcessHandler() : OrderProcessHandler("Payment Processing"), rng(std::random_device{}()) {}
    
protected:
    bool processOrder(Order* order, Customer* customer) override {
        logStep("Processing payment for customer: " + customer->getName());
        
        double totalAmount = order->calculateTotalAmount();
        logStep("Total amount: $" + std::to_string(totalAmount));
        
        // Simulate payment processing time
        logStep("Contacting payment gateway...");
        
        // Simulate payment validation (90% success rate)
        std::uniform_int_distribution<int> dist(1, 10);
        bool paymentSuccess = (dist(rng) <= 9);
        
        if (paymentSuccess) {
            logStep("Payment authorized and processed successfully");
            order->setStatus("Paid");
            
            // Remove sold plants from inventory
            logStep("Updating inventory - removing sold plants from sales floor...");
            if (removeSoldPlantsFromInventory(order)) {
                logStep("Inventory updated successfully");
                logStep("Payment confirmation sent to customer");
                return true;
            } else {
                std::cout << "[ERROR] Failed to update inventory after payment" << std::endl;
                order->setStatus("Payment Successful - Inventory Update Failed");
                return false;
            }
        } else {
            std::cout << "[ERROR] Payment failed - Card declined or insufficient funds" << std::endl;
            order->setStatus("Payment Failed");
            return false;
        }
    }
    
    /**
     * @brief Removes sold plants from the sales floor inventory
     * @param order The order containing the plants to remove
     * @return true if all plants were successfully removed, false otherwise
     */
    bool removeSoldPlantsFromInventory(Order* order) {
        InventoryManager& inventory = InventoryManager::getInstance();
        
        // Count plants by type in the order
        std::map<std::string, int> plantCounts;
        const std::vector<OrderItem*>& items = order->getOrderItems();
        
        for (OrderItem* item : items) {
            countPlantsInItem(item, plantCounts);
        }
        
        // Sell each plant type
        bool allSold = true;
        for (const auto& pair : plantCounts) {
            const std::string& plantType = pair.first;
            int quantity = pair.second;
            
            if (!inventory.sellPlants(plantType, quantity)) {
                std::cout << "[ERROR] Could not sell " << quantity << " " << plantType << std::endl;
                allSold = false;
            }
        }
        
        return allSold;
    }
    
    /**
     * @brief Recursively count plants in order items (handles bundles)
     * @param item The order item (SinglePlant or PlantBundle)
     * @param plantCounts Map to store plant type counts
     */
    void countPlantsInItem(OrderItem* item, std::map<std::string, int>& plantCounts) {
        if (!item) return;
        
        // Check if it's a SinglePlant
        SinglePlant* singlePlant = dynamic_cast<SinglePlant*>(item);
        if (singlePlant) {
            std::string plantType = singlePlant->getPlantType();
            int quantity = singlePlant->getQuantity();
            plantCounts[plantType] += quantity;
            return;
        }
        
        // Check if it's a PlantBundle
        PlantBundle* bundle = dynamic_cast<PlantBundle*>(item);
        if (bundle) {
            const std::vector<OrderItem*>& bundleItems = bundle->getItems();
            for (OrderItem* bundleItem : bundleItems) {
                countPlantsInItem(bundleItem, plantCounts);
            }
        }
    }
};

#endif