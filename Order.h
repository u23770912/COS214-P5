#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>

class OrderMemento;

class Order {
private:
    std::string orderID;
    std::string customerName;
    std::vector<std::string> items;
    double totalAmount;

    // Static list to track all Order instances
    static std::vector<Order*> allOrders;

public:
    Order(const std::string& id, const std::string& name);
    ~Order();
    void addItem(const std::string& item, double price);
    void removeItem(const std::string& item);
    void clearItems();
    std::string getId() const;
    std::string getCustomer() const;
    double getTotalAmount() const;
    const std::vector<std::string>& getItems() const;
    std::string getOrderDetails(const std::string& customerFilter = "") const;
    OrderMemento* createMemento() const;
    void restoreState(const OrderMemento* memento);

    static const std::vector<Order*>& getAllOrders();
};

#endif // ORDER_H
