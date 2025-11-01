#include "Order.h"
#include "OrderMemento.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <algorithm>

#include "OrderItem.h"
#include "SinglePlant.h"
#include "PlantBundle.h"

// Definition of static member
std::vector<Order*> Order::allOrders;

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
            
            oss << "║    R" << orderItems[i]->getPrice();
            std::ostringstream priceStr;
            priceStr << std::fixed << std::setprecision(2) << orderItems[i]->getPrice();
            for (size_t j = priceStr.str().length() + 5; j < 38; j++) oss << " ";
            oss << "║\n";
        }
    }
    
    oss << "╠════════════════════════════════════════╣\n";
    oss << "║ TOTAL: R" << totalAmount;
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
    return static_cast<int>(orderItems.size());
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

// const std::vector<std::string>& Order::getItems() const
// {
//     return items;
// }

std::string Order::getOrderDetails(const std::string& customerFilter) const {
    std::stringstream details;

    if (customerFilter.empty()) {
        details << "Order ID: " << orderId << "\n";
        details << "Customer Name: " << customerName << "\n";
        details << "Items:\n";
        for (const auto& item : items) details << "- " << item << "\n";
        details << "Total Amount: R" << totalAmount << "\n";
        return details.str();
    }

    if (customerFilter == "ALL") {
        details << "All orders for all customers:\n";
        for (auto order : allOrders) {
            details << "----------------------\n";
            details << order->getOrderDetails("");
        }
        return details.str();
    }

    details << "All orders for " << customerFilter << ":\n";
    bool found = false;
    for (auto order : allOrders) {
        if (order->getCustomerName() == customerFilter) {
            found = true;
            details << "----------------------\n";
            details << order->getOrderDetails("");
        }
    }

    if (!found) {
        details << "(no orders found for " << customerFilter << ")\n";
    }

    return details.str();
}

OrderMemento* Order::createMemento() const {
    std::stringstream state;
    
    // Save order metadata
    state << orderId << "\n";
    state << customerName << "\n";
    state << orderDate << "\n";
    state << status << "\n";
    state << totalAmount << "\n";
    
    // Save number of order items
    state << orderItems.size() << "\n";
    
    // Save each order item's description and details
    for (const auto* item : orderItems) {
        // Check if it's a SinglePlant or PlantBundle
        if (const SinglePlant* plant = dynamic_cast<const SinglePlant*>(item)) {
            state << "PLANT|" << plant->getPlantType() << "|" 
                  << plant->getQuantity() << "|" << plant->getPrice() / plant->getQuantity() 
                  << "|" << plant->getSize() << "\n";
        } else if (const PlantBundle* bundle = dynamic_cast<const PlantBundle*>(item)) {
            state << "BUNDLE|" << bundle->getName() << "|" 
                  << bundle->getQuantity() << "|" << bundle->getDiscount() 
                  << "|" << bundle->getItemCount() << "\n";
            
            // Save bundle items
            for (const auto* bundleItem : bundle->getItems()) {
                if (const SinglePlant* bplant = dynamic_cast<const SinglePlant*>(bundleItem)) {
                    state << "  BPLANT|" << bplant->getPlantType() << "|" 
                          << bplant->getQuantity() << "|" << bplant->getPrice() / bplant->getQuantity() 
                          << "|" << bplant->getSize() << "\n";
                }
            }
        }
    }
    
    return new OrderMemento(state.str());
}

void Order::restoreState(const OrderMemento* memento) {
    if (!memento) return;

    std::string s = memento->getState();
    std::stringstream state(s);
    
    // Restore order metadata
    std::getline(state, orderId);
    std::getline(state, customerName);
    std::getline(state, orderDate);
    std::getline(state, status);
    state >> totalAmount;
    state.ignore(); // eat newline after totalAmount
    
    // Clear existing order items
    for (auto* item : orderItems) {
        delete item;
    }
    orderItems.clear();
    
    // Restore number of items
    size_t numItems;
    state >> numItems;
    state.ignore(); // eat newline
    
    // Restore each order item
    for (size_t i = 0; i < numItems; i++) {
        std::string line;
        std::getline(state, line);
        
        std::stringstream lineStream(line);
        std::string itemType;
        std::getline(lineStream, itemType, '|');
        
        if (itemType == "PLANT") {
            std::string plantType, size;
            int quantity;
            double price;
            
            std::getline(lineStream, plantType, '|');
            lineStream >> quantity;
            lineStream.ignore(); // skip '|'
            lineStream >> price;
            lineStream.ignore(); // skip '|'
            std::getline(lineStream, size);
            
            SinglePlant* plant = new SinglePlant(plantType, price, quantity, size);
            orderItems.push_back(plant);
            
        } else if (itemType == "BUNDLE") {
            std::string bundleName;
            int quantity, itemCount;
            double discount;
            
            std::getline(lineStream, bundleName, '|');
            lineStream >> quantity;
            lineStream.ignore(); // skip '|'
            lineStream >> discount;
            lineStream.ignore(); // skip '|'
            lineStream >> itemCount;
            
            PlantBundle* bundle = new PlantBundle(bundleName, "Mixed", quantity, discount);
            
            // Restore bundle items
            for (int j = 0; j < itemCount; j++) {
                std::string bline;
                std::getline(state, bline);
                
                std::stringstream blineStream(bline);
                std::string bitemType;
                std::getline(blineStream, bitemType, '|');
                
                if (bitemType == "  BPLANT") {
                    std::string bplantType, bsize;
                    int bquantity;
                    double bprice;
                    
                    std::getline(blineStream, bplantType, '|');
                    blineStream >> bquantity;
                    blineStream.ignore();
                    blineStream >> bprice;
                    blineStream.ignore();
                    std::getline(blineStream, bsize);
                    
                    SinglePlant* bplant = new SinglePlant(bplantType, bprice, bquantity, bsize);
                    bundle->addItem(bplant);
                }
            }
            
            orderItems.push_back(bundle);
        }
    }
    
    // Recalculate total
    calculateTotalAmount();
}
const std::vector<Order*>& Order::getAllOrders()
{
    return allOrders;
}