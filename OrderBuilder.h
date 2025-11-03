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
    
    // Builder methods for constructing different parts of an order
    virtual void buildPlant(const std::string& plantType, int quantity) = 0;
    virtual void buildPlantPot(const std::string& potType, int quantity) = 0;
    virtual void buildBundle(const std::string& bundleType, int quantity) = 0;
    
    // Method to get the final built order
    virtual Order* getOrder() = 0;
    
    // Method to reset the builder for a new order
    virtual void reset() = 0;
};

#endif
