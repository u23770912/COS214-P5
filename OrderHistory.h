#ifndef ORDERHISTORY_H
#define ORDERHISTORY_H

#include <vector>
#include <cstddef>

class OrderMemento;
class Order;

class OrderHistory {
private:
    std::vector<OrderMemento*> history;

public:
    OrderHistory();
    ~OrderHistory();
    void addMemento(OrderMemento* memento);
    void removeMemento(std::size_t index);
    OrderMemento* getMemento(std::size_t index) const;
    void saveOrder(Order* order);
    void undo(Order* order);
};

#endif // ORDERHISTORY_H
