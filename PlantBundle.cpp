#include "PlantBundle.h"
#include <sstream>
#include <iomanip>

PlantBundle::PlantBundle(const std::string& bundleName, const std::string& bundleType, 
                         int quantity, double discount)
    : OrderItem(bundleName, 0.0, quantity), bundleType(bundleType), discountPercentage(discount) {}

PlantBundle::~PlantBundle() {
    clearItems();
}

double PlantBundle::getPrice() const {
    double basePrice = getBasePrice();
    double discountAmount = basePrice * (discountPercentage / 100.0);
    return (basePrice - discountAmount) * quantity;
}

std::string PlantBundle::getDescription() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << name << " Bundle (" << items.size() << " items, " << discountPercentage << "% off)";
    return oss.str();
}

void PlantBundle::addItem(OrderItem* item) {
    if (item) {
        items.push_back(item);
    }
}

void PlantBundle::removeItem(OrderItem* item) {
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (*it == item) {
            delete *it;
            items.erase(it);
            break;
        }
    }
}

std::vector<OrderItem*> PlantBundle::getItems() const {
    return items;
}

void PlantBundle::setDiscount(double discount) {
    if (discount >= 0.0 && discount <= 100.0) {
        discountPercentage = discount;
    }
}

double PlantBundle::getDiscount() const {
    return discountPercentage;
}

std::string PlantBundle::getBundleType() const {
    return bundleType;
}

int PlantBundle::getItemCount() const {
    return items.size();
}

void PlantBundle::clearItems() {
    for (auto* item : items) {
        delete item;
    }
    items.clear();
}

double PlantBundle::getBasePrice() const {
    double total = 0.0;
    for (const auto* item : items) {
        total += item->getPrice();
    }
    return total;
}
