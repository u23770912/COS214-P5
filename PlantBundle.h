#ifndef PLANTBUNDLE_H
#define PLANTBUNDLE_H

#include "OrderItem.h"
#include <vector>

/**
 * @brief Concrete composite class representing a bundle of plants (Composite pattern)
 * This is a Composite in the Composite pattern
 */
class PlantBundle : public OrderItem {
private:
    std::vector<OrderItem*> items;
    std::string bundleType;
    double discountPercentage;

public:
    /**
     * @brief Construct a new Plant Bundle
     * @param bundleName Name of the bundle
     * @param bundleType Type of bundle (e.g., "Holiday", "Season")
     * @param quantity Number of bundles
     * @param discount Discount percentage (0.0 to 1.0)
     */
    PlantBundle(const std::string& bundleName, const std::string& bundleType, 
                int quantity = 1, double discount = 0.0);

    /**
     * @brief Destructor
     */
    ~PlantBundle();
    
    // Override OrderItem methods
    /**
     * @brief Get the total price of the bundle after discount
     * @return Total price as double
     */
    double getPrice() const override;

    /**
     * @brief Get a formatted description of the bundle
     * @return Bundle description string
     */
    std::string getDescription() const override;
    
    // Override composite operations
    /**
     * @brief Add an item to the bundle
     * @param item Pointer to OrderItem to add
     */
    void addItem(OrderItem* item) override;

    /**
     * @brief Remove an item from the bundle
     * @param item Pointer to OrderItem to remove
     */
    void removeItem(OrderItem* item) override;

    /**
     * @brief Get all items in the bundle
     * @return Vector of OrderItem pointers
     */
    std::vector<OrderItem*> getItems() const override;
    
    // PlantBundle specific methods
    /**
     * @brief Set the discount percentage for the bundle
     * @param discountPercentage Discount value (0.0 to 1.0)
     */
    void setDiscount(double discountPercentage);

    /**
     * @brief Get the current discount percentage
     * @return Discount value as double
     */
    double getDiscount() const;

    /**
     * @brief Get the type of bundle
     * @return Bundle type string
     */
    std::string getBundleType() const;

    /**
     * @brief Get number of items in the bundle
     * @return Item count as integer
     */
    int getItemCount() const;

    /**
     * @brief Remove all items from the bundle
     */
    void clearItems();
    
    /**
     * @brief Calculate total price before discount
     * @return Base price as double
     */
    double getBasePrice() const;
};

#endif
