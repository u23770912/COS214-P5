#include "ConcreteOrderBuilder.h"
#include "Order.h"
#include "SinglePlant.h"
#include "PlantBundle.h"
#include "PlantProduct.h"
#include "PlantSpeciesProfile.h"
#include <sstream>
#include <ctime>

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
    
    // Return reference to current order WITHOUT transferring ownership
    // Builder retains ownership until explicitly reset or destroyed
    return currentOrder;
}

void ConcreteOrderBuilder::reset() {
    // Delete old order if it exists (builder retains ownership)
    delete currentOrder;
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

void ConcreteOrderBuilder::buildCustomizedPot(const std::string& potDescription, 
                                              double potPrice, int quantity) {
    if (!currentOrder) {
        reset();
    }
    
    // Create pot as a SinglePlant item with dynamic pricing
    SinglePlant* potItem = new SinglePlant(potDescription, potPrice, quantity);
    currentOrder->addOrderItem(potItem);
}

void ConcreteOrderBuilder::buildPlantWithCustomizedPot(const std::string& plantType, 
                                                       const std::string& potDescription, 
                                                       double potPrice, 
                                                       int quantity, 
                                                       const std::string& size) {
    if (!currentOrder) {
        reset();
    }
    
    // Create plant with customized pot
    SinglePlant* plant = new SinglePlant(plantType, 25.99, quantity, size);
    plant->addPot(potDescription, potPrice);
    currentOrder->addOrderItem(plant);
}

void ConcreteOrderBuilder::addBundleToOrder(PlantBundle* bundle) {
    if (!currentOrder) {
        reset();
    }
    
    if (bundle) {
        currentOrder->addOrderItem(bundle);
    }
}

void ConcreteOrderBuilder::buildPlantFromProduct(PlantProduct* plantProduct, 
                                                 int quantity, 
                                                 const std::string& size) {
    if (!currentOrder) {
        reset();
    }
    
    if (!plantProduct) {
        return;  // Safety check
    }
    
    // Get accurate plant information from PlantProduct
    std::string plantType = plantProduct->getProfile()->getSpeciesName();
    
    // Get price from profile property, default to 25.99 if not set
    std::string priceStr = plantProduct->getProfile()->getProperty("price");
    double plantPrice = priceStr.empty() ? 25.99 : std::stod(priceStr);
    
    // Create SinglePlant with accurate data from PlantProduct
    SinglePlant* plant = new SinglePlant(plantType, plantPrice, quantity, size);
    currentOrder->addOrderItem(plant);
}

void ConcreteOrderBuilder::buildPlantWithCustomizedPotFromProduct(PlantProduct* plantProduct,
                                                                  const std::string& potDescription, 
                                                                  double potPrice, 
                                                                  int quantity, 
                                                                  const std::string& size) {
    if (!currentOrder) {
        reset();
    }
    
    if (!plantProduct) {
        return;  // Safety check
    }
    
    // Get accurate plant information from PlantProduct
    std::string plantType = plantProduct->getProfile()->getSpeciesName();
    
    // Get price from profile property, default to 25.99 if not set
    std::string priceStr = plantProduct->getProfile()->getProperty("price");
    double plantPrice = priceStr.empty() ? 25.99 : std::stod(priceStr);
    
    // Create plant with customized pot using accurate plant data
    SinglePlant* plant = new SinglePlant(plantType, plantPrice, quantity, size);
    plant->addPot(potDescription, potPrice);
    currentOrder->addOrderItem(plant);
}

