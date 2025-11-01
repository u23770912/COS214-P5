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
    void addOrderItem(OrderItem* item);
    void removeOrderItem(OrderItem* item);
    std::vector<OrderItem*> getOrderItems() const;
    
    // Getters and setters
    std::string getOrderId() const;
    std::string getCustomerName() const;
    std::string getOrderDate() const;
    void setOrderDate(const std::string& date);
    std::string getStatus() const;
    void setStatus(const std::string& status);
    
    // Price calculation
    double calculateTotalAmount();
    double getTotalAmount() const;
    
    // Order operations
    std::string getOrderSummary() const;
    void clearOrder();
    int getItemCount() const;
    
    // Check if order is empty
    bool isEmpty() const;

    // Memento pattern methods
    OrderMemento* createMemento() const;
    void addItem(const std::string& item, double price);
    void removeItem(const std::string& item);
    void clearItems();
    const std::vector<std::string>& getItems() const;
    std::string getOrderDetails(const std::string& customerFilter = "") const;
    void restoreState(const OrderMemento* memento);
    static const std::vector<Order*>& getAllOrders();

};

#endif
