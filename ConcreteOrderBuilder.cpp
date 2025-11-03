#include "ConcreteOrderBuilder.h"
#include "Order.h"
#include "SinglePlant.h"
#include "PlantBundle.h"
#include "PlantProduct.h"
#include "PlantSpeciesProfile.h"
#include "Pot.h"
#include <sstream>
#include <ctime>
#include <iostream>

ConcreteOrderBuilder::ConcreteOrderBuilder(const std::string& customerName)
    : currentOrder(nullptr), customerName(customerName), orderCounter(0) {
    // Don't create an order until it's actually needed - this prevents memory leaks
}

ConcreteOrderBuilder::~ConcreteOrderBuilder() {
    // Clean up any uncompleted order that wasn't taken by the customer
    delete currentOrder;
}

std::string ConcreteOrderBuilder::generateOrderId() {
    std::ostringstream oss;
    oss << "ORD-" << time(nullptr) << "-" << (++orderCounter);
    return oss.str();
}

// ============================================================
// CORE BUILDER METHODS (New Simplified API)
// ============================================================

void ConcreteOrderBuilder::buildSinglePlant(const std::string& plantType, int quantity, const std::string& size) {
    if (!currentOrder) {
        currentOrder = new Order(generateOrderId(), customerName);
    }
    
    // Create plant with default price (will be updated if from inventory)
    SinglePlant* plant = new SinglePlant(plantType, 25.99, quantity, size);
    currentOrder->addOrderItem(plant);
    
    std::cout << "[Builder] Added " << quantity << "x " << plantType << " (" << size << ") to order" << std::endl;
}

void ConcreteOrderBuilder::buildSinglePlant(PlantProduct* plantProduct, int quantity, const std::string& size) {
    if (!currentOrder) {
        currentOrder = new Order(generateOrderId(), customerName);
    }
    
    if (!plantProduct) {
        std::cout << "[Builder] ERROR: Invalid plant product" << std::endl;
        return;
    }
    
    // Get accurate plant information from PlantProduct
    std::string plantType = plantProduct->getProfile()->getSpeciesName();
    
    // Get price from profile property, default to 25.99 if not set
    std::string priceStr = plantProduct->getProfile()->getProperty("price");
    double plantPrice = priceStr.empty() ? 25.99 : std::stod(priceStr);
    
    // Create SinglePlant with accurate data
    SinglePlant* plant = new SinglePlant(plantType, plantPrice, quantity, size);
    currentOrder->addOrderItem(plant);
    
    std::cout << "[Builder] Added " << quantity << "x " << plantType << " (R" << plantPrice << " each) to order" << std::endl;
}

PlantBundle* ConcreteOrderBuilder::buildPlantBundle(const std::string& bundleName, double discount) {
    if (!currentOrder) {
        currentOrder = new Order(generateOrderId(), customerName);
    }
    
    // Create bundle with specified discount
    PlantBundle* bundle = new PlantBundle(bundleName, "Custom", 1, discount);
    currentOrder->addOrderItem(bundle);
    
    std::cout << "[Builder] Created bundle: " << bundleName << " with " << discount << "% discount" << std::endl;
    
    return bundle;
}

void ConcreteOrderBuilder::buildPlantBundle(PlantBundle* bundle) {
    if (!currentOrder) {
        currentOrder = new Order(generateOrderId(), customerName);
    }
    
    if (bundle) {
        currentOrder->addOrderItem(bundle);
        std::cout << "[Builder] Added pre-created bundle to order" << std::endl;
    }
}

void ConcreteOrderBuilder::buildPot(Pot* pot) {
    if (!currentOrder) {
        currentOrder = new Order(generateOrderId(), customerName);
    }
    
    if (!pot) {
        std::cout << "[Builder] ERROR: Cannot add null pot" << std::endl;
        return;
    }
    
    // Create a SinglePlant item to represent the pot in the order
    // Get pot description and price
    std::ostringstream potDesc;
    std::streambuf* oldCoutBuf = std::cout.rdbuf(potDesc.rdbuf());
    pot->print();
    std::cout.rdbuf(oldCoutBuf);
    
    std::string description = potDesc.str();
    double potPrice = pot->getPrice();
    
    // Create order item for the pot
    SinglePlant* potItem = new SinglePlant("Pot: " + description, potPrice, 1);
    currentOrder->addOrderItem(potItem);
    
    std::cout << "[Builder] Added decorated pot (R" << potPrice << ") to order" << std::endl;
    
    // Delete the pot as we've extracted its information
    delete pot;
}

// ============================================================
// BUNDLE MANIPULATION METHODS
// ============================================================

void ConcreteOrderBuilder::addPlantToCurrentBundle(const std::string& plantType, 
                                                   int quantity, 
                                                   const std::string& size) {
    if (!currentOrder || currentOrder->isEmpty()) {
        std::cout << "[Builder] No current bundle to add plants to" << std::endl;
        return;
    }
    
    // Get the last order item
    auto items = currentOrder->getOrderItems();
    if (items.empty()) return;
    
    OrderItem* lastItem = items.back();
    
    // Check if it's a bundle
    if (PlantBundle* bundle = dynamic_cast<PlantBundle*>(lastItem)) {
        SinglePlant* plant = new SinglePlant(plantType, 25.99, quantity, size);
        bundle->addItem(plant);
        std::cout << "[Builder] Added " << plantType << " to current bundle" << std::endl;
    } else {
        std::cout << "[Builder] Last item is not a bundle, cannot add plant" << std::endl;
    }
}

// ============================================================
// LEGACY/COMPATIBILITY METHODS
// ============================================================

void ConcreteOrderBuilder::buildPlant(const std::string& plantType, int quantity) {
    // Legacy method - use buildSinglePlant instead
    buildSinglePlant(plantType, quantity, "medium");
}

void ConcreteOrderBuilder::buildPlantPot(const std::string& potType, int quantity) {
    // Legacy method - deprecated
    // In new system, use buildPot() with Pot* from decorator
    if (!currentOrder) {
        currentOrder = new Order(generateOrderId(), customerName);
    }
    
    SinglePlant* pot = new SinglePlant(potType + " Pot", 10.00, quantity);
    currentOrder->addOrderItem(pot);
    std::cout << "[Builder] Added legacy pot (use buildPot with Decorator pattern instead)" << std::endl;
}

void ConcreteOrderBuilder::buildBundle(const std::string& bundleType, int quantity) {
    // Legacy method - use buildPlantBundle instead
    buildPlantBundle(bundleType, 15.0);
}

Order* ConcreteOrderBuilder::getOrder() {
    // Ensure we have an order (create empty one if needed)
    if (!currentOrder) {
        currentOrder = new Order(generateOrderId(), customerName);
    }
    
    // Return reference to current order WITHOUT transferring ownership
    // Builder retains ownership until explicitly reset or destroyed
    return currentOrder;
}

void ConcreteOrderBuilder::reset() {
    // Delete old order if it exists (builder retains ownership)
    delete currentOrder;
    currentOrder = new Order(generateOrderId(), customerName);
    std::cout << "[Builder] Order reset - new order created" << std::endl;
}

// Utility methods
bool ConcreteOrderBuilder::hasCurrentOrder() const {
    return currentOrder != nullptr && !currentOrder->isEmpty();
}

std::string ConcreteOrderBuilder::getCurrentCustomerName() const {
    return customerName;
}

