#ifndef ORDERBUILDER_H
#define ORDERBUILDER_H

#include <string>

class Order; // Forward declaration
class OrderItem; // Forward declaration

/**
 * @brief Abstract Builder class for creating orders
 */
class OrderBuilder {
public:
    virtual ~OrderBuilder() {}
    
    /**
     * @brief Build a plant order item
     * @param plantType Type of plant to build
     * @param quantity Number of plants to include
     */
    virtual void buildPlant(const std::string& plantType, int quantity) = 0;

    /**
     * @brief Build a plant pot order item
     * @param potType Type of pot to build
     * @param quantity Number of pots to include
     */
    virtual void buildPlantPot(const std::string& potType, int quantity) = 0;

    /**
     * @brief Build a bundle order item
     * @param bundleType Type of bundle to build
     * @param quantity Number of bundles to include
     */
    virtual void buildBundle(const std::string& bundleType, int quantity) = 0;
    
    /**
     * @brief Get the constructed order
     * @return Pointer to the built Order object
     */
    virtual Order* getOrder() = 0;
    
    /**
     * @brief Reset the builder to start a new order
     */
    virtual void reset() = 0;
};

#endif
