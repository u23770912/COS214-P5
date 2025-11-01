#ifndef ORDERDIRECTOR_H
#define ORDERDIRECTOR_H

#include <string>
#include <vector>

class OrderBuilder; // Forward declaration
class Order; // Forward declaration

/**
 * @brief Director class for the Builder Design Pattern
 * 
 * The OrderDirector encapsulates the construction logic for different types of orders.
 * It coordinates the step-by-step building process through the Builder interface,
 * allowing the Customer to simply call construct() without managing the building details.
 */
class OrderDirector {
private:
    OrderBuilder* builder;
    
public:
    OrderDirector();
    OrderDirector(OrderBuilder* builder);
    ~OrderDirector();
    
    /**
     * @brief Set the builder to be used for construction
     * @param builder Pointer to the builder instance
     */
    void setBuilder(OrderBuilder* builder);
    
    /**
     * @brief Get the current builder
     * @return Pointer to the current builder
     */
    OrderBuilder* getBuilder() const;
    
    /**
     * @brief Main construction method - encapsulates the entire order building process
     * This is the single entry point that Customer calls
     * @return Pointer to the constructed Order
     */
    Order* construct();
    
    /**
     * @brief Construct a simple plant order (one type of plant)
     * @param plantType Type of plant to order
     * @param quantity Number of plants
     * @return Pointer to the constructed Order
     */
    Order* constructSimplePlantOrder(const std::string& plantType, int quantity);
    
    /**
     * @brief Construct a plant order with pot
     * @param plantType Type of plant to order
     * @param potType Type of pot to include
     * @param quantity Number of plant+pot combinations
     * @return Pointer to the constructed Order
     */
    Order* constructPlantWithPotOrder(const std::string& plantType, 
                                      const std::string& potType, int quantity);
    
    /**
     * @brief Construct a custom bundle order
     * @param bundleName Name of the bundle
     * @param plantTypes Vector of plant types to include
     * @param quantities Vector of quantities for each plant type
     * @param discount Discount percentage to apply
     * @return Pointer to the constructed Order
     */
    Order* constructBundleOrder(const std::string& bundleName,
                               const std::vector<std::string>& plantTypes,
                               const std::vector<int>& quantities,
                               double discount);
    
    /**
     * @brief Construct a premium garden starter kit
     * Predefined combination of popular plants with accessories
     * @return Pointer to the constructed Order
     */
    Order* constructGardenStarterKit();
    
    /**
     * @brief Construct a succulent collection bundle
     * Predefined collection of various succulent plants
     * @return Pointer to the constructed Order
     */
    Order* constructSucculentCollection();
    
private:
    /**
     * @brief Validate that builder is set and ready
     * @return true if builder is valid, false otherwise
     */
    bool validateBuilder() const;
    
    /**
     * @brief Reset the builder for a new construction
     */
    void resetBuilder();
};

#endif // ORDERDIRECTOR_H
