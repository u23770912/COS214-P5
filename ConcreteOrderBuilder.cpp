#include "ConcreteOrderBuilder.h"
#include "Order.h"
#include "SinglePlant.h"
#include "PlantBundle.h"
#include <sstream>
#include <ctime>

ConcreteOrderBuilder::ConcreteOrderBuilder(const std::string& customerName)
    : currentOrder(nullptr), customerName(customerName), orderCounter(0) {
    reset();
}

ConcreteOrderBuilder::~ConcreteOrderBuilder() {
    // Don't delete currentOrder here - it's returned to the customer
}

std::string ConcreteOrderBuilder::generateOrderId() {
    std::ostringstream oss;
    oss << "ORD-" << time(nullptr) << "-" << (++orderCounter);
    return oss.str();
}

void ConcreteOrderBuilder::buildPlant(const std::string& plantType, int quantity) {
    if (!currentOrder) {
        reset();
    }
    
    SinglePlant* plant = new SinglePlant(plantType, 25.99, quantity);
    currentOrder->addOrderItem(plant);
}

void ConcreteOrderBuilder::buildPlantPot(const std::string& potType, int quantity) {
    // For now, just add as a separate item
    // In a real system, this might be attached to the last plant added
    if (!currentOrder) {
        reset();
    }
    
    SinglePlant* pot = new SinglePlant(potType + " Pot", 12.99, quantity);
    currentOrder->addOrderItem(pot);
}

void ConcreteOrderBuilder::buildBundle(const std::string& bundleType, int quantity) {
    if (!currentOrder) {
        reset();
    }
    
    PlantBundle* bundle = new PlantBundle(bundleType, bundleType, quantity, 15.0);
    currentOrder->addOrderItem(bundle);
}

Order* ConcreteOrderBuilder::getOrder() {
    if (!currentOrder) {
        reset();
    }
    return currentOrder;
}

void ConcreteOrderBuilder::reset() {
    // Don't delete old order - caller owns it after getOrder()
    currentOrder = new Order(generateOrderId(), customerName);
}

void ConcreteOrderBuilder::buildPlantWithPot(const std::string& plantType, 
                                            const std::string& potType, 
                                            int quantity, const std::string& size) {
    if (!currentOrder) {
        reset();
    }
    
    SinglePlant* plant = new SinglePlant(plantType, 25.99, quantity, size);
    plant->addPot(potType, 12.99);
    currentOrder->addOrderItem(plant);
}

void ConcreteOrderBuilder::buildCustomBundle(const std::string& bundleName, 
                                             const std::string& bundleType,
                                             double discount) {
    if (!currentOrder) {
        reset();
    }
    
    PlantBundle* bundle = new PlantBundle(bundleName, bundleType, 1, discount);
    currentOrder->addOrderItem(bundle);
}

void ConcreteOrderBuilder::addPlantToCurrentBundle(const std::string& plantType, 
                                                   int quantity, 
                                                   const std::string& size) {
    if (!currentOrder || currentOrder->isEmpty()) {
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
    }
}

void ConcreteOrderBuilder::addPlantWithPotToCurrentBundle(const std::string& plantType, 
                                                          const std::string& potType, 
                                                          int quantity,
                                                          const std::string& size) {
    if (!currentOrder || currentOrder->isEmpty()) {
        return;
    }
    
    auto items = currentOrder->getOrderItems();
    if (items.empty()) return;
    
    OrderItem* lastItem = items.back();
    
    if (PlantBundle* bundle = dynamic_cast<PlantBundle*>(lastItem)) {
        SinglePlant* plant = new SinglePlant(plantType, 25.99, quantity, size);
        plant->addPot(potType, 12.99);
        bundle->addItem(plant);
    }
}

bool ConcreteOrderBuilder::hasCurrentOrder() const {
    return currentOrder != nullptr && !currentOrder->isEmpty();
}

std::string ConcreteOrderBuilder::getCurrentCustomerName() const {
    return customerName;
}
