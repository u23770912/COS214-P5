#include "Order.h"
#include "OrderMemento.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <algorithm>

Order::Order(const std::string& orderId, const std::string& customerName)
    : orderId(orderId), customerName(customerName), totalAmount(0.0), status("Pending") {
    // Generate timestamp
    time_t now = time(0);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    orderDate = std::string(buf);
    allOrders.push_back(this);
}

Order::~Order() {
    // Delete all order items
    for (auto* item : orderItems) {
        delete item;
    }
    orderItems.clear();

    auto it = std::find(allOrders.begin(), allOrders.end(), this);
    if (it != allOrders.end()) allOrders.erase(it);
}

void Order::addOrderItem(OrderItem* item) {
    if (item) {
        orderItems.push_back(item);
        calculateTotalAmount();
    }
}

void Order::removeOrderItem(OrderItem* item) {
    for (auto it = orderItems.begin(); it != orderItems.end(); ++it) {
        if (*it == item) {
            delete *it;
            orderItems.erase(it);
            calculateTotalAmount();
            break;
        }
    }
}

std::vector<OrderItem*> Order::getOrderItems() const {
    return orderItems;
}

std::string Order::getOrderId() const {
    return orderId;
}

std::string Order::getCustomerName() const {
    return customerName;
}

std::string Order::getOrderDate() const {
    return orderDate;
}

void Order::setOrderDate(const std::string& date) {
    orderDate = date;
}

std::string Order::getStatus() const {
    return status;
}

void Order::setStatus(const std::string& newStatus) {
    status = newStatus;
}

double Order::calculateTotalAmount() {
    totalAmount = 0.0;
    for (const auto* item : orderItems) {
        totalAmount += item->getPrice();
    }
    return totalAmount;
}

double Order::getTotalAmount() const {
    return totalAmount;
}

std::string Order::getOrderSummary() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    
    oss << "\n╔════════════════════════════════════════╗\n";
    oss << "║           ORDER SUMMARY               ║\n";
    oss << "╠════════════════════════════════════════╣\n";
    oss << "║ Order ID: " << orderId;
    for (size_t i = orderId.length(); i < 27; i++) oss << " ";
    oss << "║\n";
    
    oss << "║ Customer: " << customerName;
    for (size_t i = customerName.length(); i < 27; i++) oss << " ";
    oss << "║\n";
    
    oss << "║ Date: " << orderDate;
    for (size_t i = orderDate.length(); i < 31; i++) oss << " ";
    oss << "║\n";
    
    oss << "║ Status: " << status;
    for (size_t i = status.length(); i < 29; i++) oss << " ";
    oss << "║\n";
    
    oss << "╠════════════════════════════════════════╣\n";
    oss << "║ ITEMS:                                ║\n";
    
    if (orderItems.empty()) {
        oss << "║   (No items)                          ║\n";
    } else {
        for (size_t i = 0; i < orderItems.size(); i++) {
            oss << "║ " << (i+1) << ". " << orderItems[i]->getDescription();
            std::string desc = orderItems[i]->getDescription();
            for (size_t j = desc.length() + 3; j < 38; j++) oss << " ";
            oss << "║\n";
            
            oss << "║    $" << orderItems[i]->getPrice();
            std::ostringstream priceStr;
            priceStr << std::fixed << std::setprecision(2) << orderItems[i]->getPrice();
            for (size_t j = priceStr.str().length() + 5; j < 38; j++) oss << " ";
            oss << "║\n";
        }
    }
    
    oss << "╠════════════════════════════════════════╣\n";
    oss << "║ TOTAL: $" << totalAmount;
    std::ostringstream totalStr;
    totalStr << std::fixed << std::setprecision(2) << totalAmount;
    for (size_t i = totalStr.str().length() + 9; i < 38; i++) oss << " ";
    oss << "║\n";
    oss << "╚════════════════════════════════════════╝\n";
    
    return oss.str();
}

void Order::clearOrder() {
    for (auto* item : orderItems) {
        delete item;
    }
    orderItems.clear();
    totalAmount = 0.0;
    status = "Pending";
}

int Order::getItemCount() const {
    return orderItems.size();
}

bool Order::isEmpty() const {
    return orderItems.empty();
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

const std::vector<std::string>& Order::getItems() const
{
    return items;
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