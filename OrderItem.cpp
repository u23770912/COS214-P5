#include "OrderItem.h"
#include <stdexcept>

OrderItem::OrderItem(const std::string& name, double price, int quantity)
    : name(name), price(price), quantity(quantity) {}

OrderItem::~OrderItem() {}

std::string OrderItem::getName() const {
    return name;
}

int OrderItem::getQuantity() const {
    return quantity;
}

void OrderItem::setQuantity(int newQuantity) {
    if (newQuantity > 0) {
        quantity = newQuantity;
    }
}

// Default implementation for leaf nodes - throws exception
void OrderItem::addItem(OrderItem* item) {
    // Leaf nodes don't support adding items
    (void)item; // Suppress unused parameter warning
}

void OrderItem::removeItem(OrderItem* item) {
    // Leaf nodes don't support removing items
    (void)item; // Suppress unused parameter warning
}

std::vector<OrderItem*> OrderItem::getItems() const {
    // Leaf nodes return empty vector
    return std::vector<OrderItem*>();
}
