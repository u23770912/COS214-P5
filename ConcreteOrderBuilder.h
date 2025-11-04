#ifndef CONCRETEORDERBUILDER_H
#define CONCRETEORDERBUILDER_H

#include "OrderBuilder.h"
#include "Order.h"
#include "SinglePlant.h"
#include "PlantBundle.h"
#include <string>

class Order;
class PlantProduct;
class Pot;

/**
 * @brief Concrete implementation of OrderBuilder
 * @details Simplified builder with three core operations:
 *          1. Build Single Plant
 *          2. Build Plant Bundle
 *          3. Build Pot (using Decorator pattern)
 * 
 * The builder maintains the current order and allows customers to:
 * - Add individual plants
 * - Create bundles with multiple plants
 * - Add decorated pots to their order
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
    
    // ============================================================
    // CORE BUILDER METHODS (Simplified API)
    // ============================================================
    
    /**
     * @brief Build a single plant and add to order
     * @param plantType The type/species of plant
     * @param quantity Number of plants
     * @param size Size of plant (small/medium/large)
     */
    void buildSinglePlant(const std::string& plantType, int quantity, const std::string& size = "medium");
    
    /**
     * @brief Build a single plant from PlantProduct (accurate pricing)
     * @param plantProduct Pointer to PlantProduct from inventory
     * @param quantity Number of plants
     * @param size Size of plant (small/medium/large)
     */
    void buildSinglePlant(PlantProduct* plantProduct, int quantity, const std::string& size = "medium");
    
    /**
     * @brief Build a plant bundle and add to order
     * @param bundleName Name of the bundle
     * @param discount Discount percentage (0-100)
     * @return Pointer to the created bundle (for adding plants to it)
     */
    PlantBundle* buildPlantBundle(const std::string& bundleName, double discount = 0.0);
    
    /**
     * @brief Add pre-created bundle to order
     * @param bundle Pointer to PlantBundle (takes ownership)
     */
    void buildPlantBundle(PlantBundle* bundle);
    
    /**
     * @brief Build a pot with decorations and add to order
     * @param pot Fully decorated Pot* created using Decorator pattern
     * @details The pot parameter should be created using:
     *          1. Base pot from factory (ClayPot, PlasticPot, etc.)
     *          2. Decorated with ColorDecorator, TextureDecorator, etc.
     *          Example:
     *            Pot* pot = new ClayPot("Medium", "Round", true);
     *            pot = new ColorDecorator(pot, "Terracotta");
     *            builder->buildPot(pot);
     */
    void buildPot(Pot* pot);
    
    // ============================================================
    // BUNDLE MANIPULATION METHODS
    // ============================================================
    
    /**
     * @brief Add plant to the most recently created bundle
     * @param plantType The type/species of plant
     * @param quantity Number of plants
     * @param size Size of plant
     */
    void addPlantToCurrentBundle(const std::string& plantType, int quantity, 
                                const std::string& size = "medium");
    
    // ============================================================
    // LEGACY/COMPATIBILITY METHODS
    // ============================================================
    
    // Override OrderBuilder methods (for backward compatibility)
    void buildPlant(const std::string& plantType, int quantity) override;
    void buildPlantPot(const std::string& potType, int quantity) override;
    void buildBundle(const std::string& bundleType, int quantity) override;
    
    Order* getOrder() override;
    void reset() override;
    
    // Utility methods
    bool hasCurrentOrder() const;
    std::string getCurrentCustomerName() const;
};

#endif