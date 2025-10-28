#include "Order.h"
#include <sstream>
#include <iomanip>
#include <ctime>

Order::Order(const std::string& orderId, const std::string& customerName)
    : orderId(orderId), customerName(customerName), totalAmount(0.0), status("Pending") {
    // Generate timestamp
    time_t now = time(0);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    orderDate = std::string(buf);
}

Order::~Order() {
    // Delete all order items
    for (auto* item : orderItems) {
        delete item;
    }
    orderItems.clear();
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
