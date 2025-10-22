#ifndef ORDERMEMENTO_H
#define OrderHistor_CH

#include "OrderMemento.h"
#include "Order.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class OrderHistory {
private:
    vector<OrderMemento*> history;

public:
    OrderHistory();
    ~OrderHistory();
    void saveOrder(Order* order);
    void undo(Order* order);

    void addMemento(OrderMemento* memento);
    void removeMemento(int id);
    OrderMemento* getMemento(int index) const;
};

#endif // OrderHistor_CPP