#include "Order.h"
#include "OrderMemento.h"

#include <sstream>
#include <algorithm>

// define static member
std::vector<Order*> Order::allOrders;

Order::Order(const std::string& id, const std::string& name)
    : orderID(id), customerName(name), totalAmount(0.0)
{
    allOrders.push_back(this);
}

Order::~Order()
{
    auto it = std::find(allOrders.begin(), allOrders.end(), this);
    if (it != allOrders.end()) allOrders.erase(it);
}

void Order::addItem(const std::string& item, double price)
{
    items.push_back(item);
    totalAmount += price;
}

void Order::removeItem(const std::string& item)
{
    auto it = std::find(items.begin(), items.end(), item);
    if (it != items.end())
    {
        items.erase(it);
        // NOTE: we don't track item prices per-item, so totalAmount is not adjusted here.
        // If you want to deduct price when removing, store a vector of (item,price) pairs.
    }
}

void Order::clearItems()
{
    items.clear();
    totalAmount = 0.0;
}

std::string Order::getId() const
{
    return orderID;
}

std::string Order::getCustomer() const
{
    return customerName;
}

double Order::getTotalAmount() const
{
    return totalAmount;
}

const std::vector<std::string>& Order::getItems() const
{
    return items;
}

std::string Order::getOrderDetails(const std::string& customerFilter) const
{
    std::stringstream details;

    if (customerFilter.empty())
    {
        // details for this single order
        details << "Order ID: " << orderID << "\n";
        details << "Customer Name: " << customerName << "\n";
        details << "Items:\n";
        for (const auto& item : items) details << "- " << item << "\n";
        details << "Total Amount: R" << totalAmount << "\n";
        return details.str();
    }

    if (customerFilter == "ALL")
    {
        details << "All orders for all customers:\n";
        for (auto order : allOrders)
        {
            details << "----------------------\n";
            details << order->getOrderDetails(""); // single-order details
        }
        return details.str();
    }

    // Filter by specific customer name
    details << "All orders for " << customerFilter << ":\n";
    bool found = false;
    for (auto order : allOrders)
    {
        if (order->getCustomer() == customerFilter)
        {
            found = true;
            details << "----------------------\n";
            details << order->getOrderDetails("");
        }
    }

    if (!found)
    {
        details << "(no orders found for " << customerFilter << ")\n";
    }

    return details.str();
}

OrderMemento* Order::createMemento() const
{
    std::stringstream state;
    state << orderID << "\n" << customerName << "\n" << totalAmount << "\n";
    for (const auto& item : items)
    {
        state << item << "\n";
    }

    return new OrderMemento(state.str());
}

void Order::restoreState(const OrderMemento* memento)
{
    if (!memento) return;

    std::string s = memento->getState();
    std::stringstream state(s);
    std::getline(state, orderID);
    std::getline(state, customerName);
    state >> totalAmount;
    state.ignore(); // eat newline after totalAmount

    items.clear();
    std::string item;
    while (std::getline(state, item))
    {
        if (!item.empty()) items.push_back(item);
    }
}

const std::vector<Order*>& Order::getAllOrders()
{
    return allOrders;
}
