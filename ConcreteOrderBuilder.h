#ifndef CONCRETEORDERBUILDER_H
#define CONCRETEORDERBUILDER_H

#include "OrderBuilder.h"
#include "Order.h"
#include "SinglePlant.h"
#include "PlantBundle.h"
#include <string>

class Order;

/**
 * @brief Concrete implementation of OrderBuilder
 * Builds orders with plants, plant pots, and bundles
 */
class ConcreteOrderBuilder : public OrderBuilder {
private:
    Order* currentOrder;
    std::string customerName;
    int orderCounter;
    
    // Helper method to generate unique order ID
    std::string generateOrderId();

public:
    ConcreteOrderBuilder(const std::string& customerName);
    ~ConcreteOrderBuilder();
    
    // Override OrderBuilder methods
    void buildPlant(const std::string& plantType, int quantity) override;
    void buildPlantPot(const std::string& potType, int quantity) override;
    void buildBundle(const std::string& bundleType, int quantity) override;
    
    Order* getOrder() override;
    void reset() override;
    
    // Additional methods for building complex orders
    void buildPlantWithPot(const std::string& plantType, const std::string& potType, 
                          int quantity, const std::string& size = "medium");
    void buildCustomBundle(const std::string& bundleName, const std::string& bundleType,
                          double discount = 0.0);
    
    // Method to add individual items to current bundle
    void addPlantToCurrentBundle(const std::string& plantType, int quantity, 
                                const std::string& size = "medium");
    void addPlantWithPotToCurrentBundle(const std::string& plantType, 
                                       const std::string& potType, int quantity,
                                       const std::string& size = "medium");
    
    // Utility methods
    bool hasCurrentOrder() const;
    std::string getCurrentCustomerName() const;
};

#endif