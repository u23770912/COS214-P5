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
    PlantBundle(const std::string& bundleName, const std::string& bundleType, 
                int quantity = 1, double discount = 0.0);
    ~PlantBundle();
    
    // Override OrderItem methods
    double getPrice() const override;
    std::string getDescription() const override;
    
    // Override composite operations
    void addItem(OrderItem* item) override;
    void removeItem(OrderItem* item) override;
    std::vector<OrderItem*> getItems() const override;
    
    // PlantBundle specific methods
    void setDiscount(double discountPercentage);
    double getDiscount() const;
    std::string getBundleType() const;
    int getItemCount() const;
    void clearItems();
    
    // Calculate total price of all items before discount
    double getBasePrice() const;
};

#endif
