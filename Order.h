#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class OrderMemento;

class Order {
private:
    string orderID;
    string customerName;
    vector<string> items;
    double totalAmount;
    
public:
    Order(const string& id, const string& name);
    ~Order();
    void addItem(const string& item, double price);
    void removeItem(const string& item);
    double getTotalAmount() const;
    string getOrderDetails() const;
    OrderMemento* createMemento() const;
    void restoreState(const OrderMemento* memento);
};

#endif // ORDER_H