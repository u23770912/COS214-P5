#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include "OrderItem.h"

class OrderMemento;

/**
 * @brief Order class that contains order items and manages the order
 */
class Order {
private:
    std::string orderId;
    std::vector<OrderItem*> orderItems;
    std::string customerName;
    std::string orderDate;
    double totalAmount;
    std::string status;
    std::vector<std::string> items;

    static std::vector<Order*> allOrders;

public:
    Order(const std::string& orderId, const std::string& customerName);
    ~Order();
    
    // Order management methods
    /**
     * @brief Add an item to the order
     * @param item OrderItem pointer to add
     */
    void addOrderItem(OrderItem* item);

    /**
     * @brief Remove an item from the order
     * @param item OrderItem pointer to remove
     */
    void removeOrderItem(OrderItem* item);

    /**
     * @brief Get all items in the order
     * @return Vector of OrderItem pointers
     */
    std::vector<OrderItem*> getOrderItems() const;
    
    // Getters and setters
    /**
     * @brief Get the order's unique identifier
     * @return Order ID string
     */
    std::string getOrderId() const;

    /**
     * @brief Get the customer's name
     * @return Customer name string
     */
    std::string getCustomerName() const;

    /**
     * @brief Get the order date
     * @return Order date string
     */
    std::string getOrderDate() const;

    /**
     * @brief Set the order date
     * @param date Date string to set
     */
    void setOrderDate(const std::string& date);

    /**
     * @brief Get the order status
     * @return Status string
     */
    std::string getStatus() const;

    /**
     * @brief Set the order status
     * @param status Status string to set
     */
    void setStatus(const std::string& status);
    
    // Price calculation methods
    /**
     * @brief Calculate the total amount for all items
     * @return Total amount as double
     */
    double calculateTotalAmount();

    /**
     * @brief Get the current total amount
     * @return Current total as double
     */
    double getTotalAmount() const;
    
    // Discount management
    double calculateAutomaticDiscount() const;
    double getDiscountPercentage() const;
    double getTotalBeforeDiscount() const;
    
    // Order operations
    /**
     * @brief Get a formatted summary of the order
     * @return Order summary string
     */
    std::string getOrderSummary() const;

    /**
     * @brief Clear all items from the order
     */
    void clearOrder();

    /**
     * @brief Get the number of items in the order
     * @return Number of items
     */
    int getItemCount() const;
    
    /**
     * @brief Check if the order has no items
     * @return true if order is empty, false otherwise
     */
    bool isEmpty() const;

    // Memento pattern methods (temporarily disabled)
    OrderMemento* createMemento() const;
    void restoreState(const OrderMemento* memento);
    
    void addItem(const std::string& item, double price);
    void removeItem(const std::string& item);
    void clearItems();
    const std::vector<std::string>& getItems() const;
    std::string getOrderDetails(const std::string& customerFilter = "") const;
    static const std::vector<Order*>& getAllOrders();

};

#endif
