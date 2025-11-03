#ifndef ORDERITEM_H
#define ORDERITEM_H

#include <string>
#include <vector>

/**
 * @brief Abstract base class for order items (Composite pattern)
 * This is the Component in the Composite pattern
 */
class OrderItem {
protected:
    std::string name;
    double price;
    int quantity;

public:
    OrderItem(const std::string& name, double price, int quantity);
    virtual ~OrderItem();
    
    // Common interface for all order items
    virtual double getPrice() const = 0;
    virtual std::string getName() const;
    virtual int getQuantity() const;
    virtual void setQuantity(int quantity);
    
    // Composite operations (default implementations for leaf nodes)
    virtual void addItem(OrderItem* item);
    virtual void removeItem(OrderItem* item);
    virtual std::vector<OrderItem*> getItems() const;
    
    // Display/description method
    virtual std::string getDescription() const = 0;
};

#endif
