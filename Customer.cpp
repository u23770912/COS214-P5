#include "Customer.h"
#include "ConcreteOrderBuilder.h"
#include "OrderDirector.h"
#include "PlaceOrderCommand.h"
#include "Order.h"
#include "InventoryManager.h"
#include "PlantProduct.h"
#include "CustomerObserver.h"
#include "BridgeDP/PlantSpeciesProfile.h"
#include <iostream>
#include <iomanip>

Customer::Customer(const std::string& name, const std::string& email, const std::string& cellPhone)
    : name(name), email(email), cellPhone(cellPhone), orderBuilder(nullptr), orderDirector(nullptr), orderProduct(nullptr), placeOrderCommand(nullptr) {
    // Initialize the order builder
    orderBuilder = new ConcreteOrderBuilder(name);
    
    // Initialize the order director with the builder
    orderDirector = new OrderDirector(orderBuilder);
}

Customer::~Customer() {
    delete orderDirector;
    delete orderBuilder;
    delete orderProduct;
    delete placeOrderCommand;
}

std::string Customer::getName() const {
    return name;
}

std::string Customer::getEmail() const {
    return email;
}

std::string Customer::getCellPhone() const {
    return cellPhone;
}

Order* Customer::createOrder() {
    std::cout << "\n=== Welcome " << name << "! Let's create your order ===" << std::endl;
    
    // Clean up any previous order
    delete orderProduct;
    delete placeOrderCommand;
    orderProduct = nullptr;
    placeOrderCommand = nullptr;
    
    // Reset the builder for a new order
    orderBuilder->reset();
    
    std::cout << "✓ New order session started!" << std::endl;
    std::cout << "Use the interactive methods to build your order:" << std::endl;
    std::cout << "1. displayAvailableItems() - See what's available" << std::endl;
    std::cout << "2. addPlantToOrder(index, quantity) - Add plants" << std::endl;
    std::cout << "3. addBundleToOrder(name, indices) - Create bundles" << std::endl;
    std::cout << "4. viewCurrentOrder() - Review your order" << std::endl;
    std::cout << "5. finalizeOrder() - Complete the order" << std::endl;
    
    return nullptr; // Order will be created when finalized
}

bool Customer::executeOrder() {
    if (!orderProduct) {
        std::cout << "[ERROR] No finalized order to execute. Please finalize your order first." << std::endl;
        return false;
    }
    
    if (orderProduct->isEmpty()) {
        std::cout << "[ERROR] Cannot execute empty order." << std::endl;
        return false;
    }
    
    std::cout << "\n=== EXECUTING ORDER ===" << std::endl;
    std::cout << "Customer: " << name << " (" << email << ")" << std::endl;
    std::cout << "Order Summary:\n" << orderProduct->getOrderSummary() << std::endl;
    
    // Step 1: Request staff validation FIRST (as designed!)
    std::cout << "\n[STEP 1] Requesting Staff Validation..." << std::endl;
    if (!requestValidation(orderProduct)) {
        std::cout << "[ERROR] Staff validation failed - cannot proceed with order" << std::endl;
        return false;
    }
    
    std::cout << "[SUCCESS] Staff validation completed!" << std::endl;
    
    // Step 2: Create and execute the PlaceOrderCommand (after staff approval)
    std::cout << "\n[STEP 2] Processing order through payment system..." << std::endl;
    delete placeOrderCommand;
    placeOrderCommand = new PlaceOrderCommand(orderProduct, this);
    
    try {
        placeOrderCommand->execute();
        std::cout << "[SUCCESS] Order executed successfully!" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cout << "[ERROR] Order execution failed: " << e.what() << std::endl;
        return false;
    }
}

void Customer::displayAvailableItems() {
    std::cout << "\n=== AVAILABLE PLANTS IN NURSERY ===" << std::endl;
    
    auto availablePlants = getAvailablePlantsFromInventory();
    
    if (availablePlants.empty()) {
        std::cout << "[ERROR] Sorry! No plants are currently available for sale." << std::endl;
        std::cout << "Please check back later or contact staff." << std::endl;
        return;
    }
    
    std::cout << "We have " << availablePlants.size() << " plants ready for sale:" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    for (size_t i = 0; i < availablePlants.size(); ++i) {
        displayPlantDetails(availablePlants[i], i + 1);
    }
    
    std::cout << std::string(60, '-') << std::endl;
    std::cout << "[INFO] To add a plant: addPlantToOrder(number, quantity)" << std::endl;
    std::cout << "[INFO] Example: addPlantToOrder(1, 2) adds 2 of item #1" << std::endl;
}

bool Customer::addPlantToOrder(int plantIndex, int quantity) {
    auto availablePlants = getAvailablePlantsFromInventory();
    
    // Validate input
    if (plantIndex < 1 || plantIndex > static_cast<int>(availablePlants.size())) {
        std::cout << "[ERROR] Invalid plant number. Please choose between 1 and " 
                  << availablePlants.size() << std::endl;
        return false;
    }
    
    if (quantity <= 0) {
        std::cout << "[ERROR] Quantity must be greater than 0." << std::endl;
        return false;
    }
    
    PlantProduct* selectedPlant = availablePlants[plantIndex - 1];
    std::string plantType = selectedPlant->getProfile()->getSpeciesName();
    
    std::cout << "\n[SUCCESS] Adding " << quantity << "x " << plantType 
              << " to your order..." << std::endl;
    
    // Notify observers of customer interaction
    notifyInteraction("PlantSelection", 
        "Customer selected " + std::to_string(quantity) + "x " + plantType);
    
    // Use the builder to add the plant
    orderBuilder->buildPlant(plantType, quantity);
    
    std::cout << "[COMPLETE] Plant added successfully!" << std::endl;
    return true;
}

bool Customer::addBundleToOrder(const std::string& bundleName, const std::vector<int>& plantIndices, double discount) {
    auto availablePlants = getAvailablePlantsFromInventory();
    
    if (plantIndices.empty()) {
        std::cout << "[ERROR] Bundle must contain at least one plant." << std::endl;
        return false;
    }
    
    // Validate all indices
    for (int index : plantIndices) {
        if (index < 1 || index > static_cast<int>(availablePlants.size())) {
            std::cout << "[ERROR] Invalid plant number " << index << ". Please choose between 1 and " 
                      << availablePlants.size() << std::endl;
            return false;
        }
    }
    
    std::cout << "\n[BUNDLE] Creating bundle: " << bundleName << " with " << discount << "% discount" << std::endl;
    
    // Notify observers of bundle creation
    notifyInteraction("BundleCreation", 
        "Customer creating " + bundleName + " with " + std::to_string(plantIndices.size()) + " plants");
    
    // Use builder to create bundle
    ConcreteOrderBuilder* concreteBuilder = dynamic_cast<ConcreteOrderBuilder*>(orderBuilder);
    if (concreteBuilder) {
        concreteBuilder->buildCustomBundle(bundleName, "Mixed", discount);
        
        // Add each plant to the bundle
        for (int index : plantIndices) {
            PlantProduct* plant = availablePlants[index - 1];
            std::string plantType = plant->getProfile()->getSpeciesName();
            concreteBuilder->addPlantToCurrentBundle(plantType, 1);
            std::cout << "  [OK] Added " << plantType << " to bundle" << std::endl;
        }
    }
    
    std::cout << "[BUNDLE] Bundle created successfully!" << std::endl;
    return true;
}

void Customer::viewCurrentOrder() {
    ConcreteOrderBuilder* concreteBuilder = dynamic_cast<ConcreteOrderBuilder*>(orderBuilder);
    if (!concreteBuilder || !concreteBuilder->hasCurrentOrder()) {
        std::cout << "\n[ORDER] Your order is currently empty." << std::endl;
        std::cout << "[INFO] Use displayAvailableItems() to see what's available!" << std::endl;
        return;
    }
    
    std::cout << "\n=== YOUR CURRENT ORDER ===" << std::endl;
    // Get a temporary order to display
    Order* tempOrder = orderBuilder->getOrder();
    if (tempOrder && !tempOrder->isEmpty()) {
        std::cout << tempOrder->getOrderSummary() << std::endl;
        std::cout << "[TOTAL] Total: $" << std::fixed << std::setprecision(2) 
                  << tempOrder->getTotalAmount() << std::endl;
    } else {
        std::cout << "[ORDER] Order is empty." << std::endl;
    }
}

bool Customer::finalizeOrder() {
    ConcreteOrderBuilder* concreteBuilder = dynamic_cast<ConcreteOrderBuilder*>(orderBuilder);
    if (!concreteBuilder || !concreteBuilder->hasCurrentOrder()) {
        std::cout << "[ERROR] No order to finalize. Add some plants first!" << std::endl;
        return false;
    }
    
    // Get the final order from builder
    delete orderProduct;
    orderProduct = orderBuilder->getOrder();
    
    if (!orderProduct || orderProduct->isEmpty()) {
        std::cout << "[ERROR] Cannot finalize empty order." << std::endl;
        return false;
    }
    
    std::cout << "\n=== ORDER FINALIZED ===" << std::endl;
    std::cout << orderProduct->getOrderSummary() << std::endl;
    std::cout << "[TOTAL] Final Total: $" << std::fixed << std::setprecision(2) 
              << orderProduct->getTotalAmount() << std::endl;
    std::cout << "\n[SUCCESS] Order is ready for execution!" << std::endl;
    std::cout << "[INFO] Use executeOrder() to place your order with staff." << std::endl;
    
    return true;
}

// Public method to get access to the builder for building the order
ConcreteOrderBuilder* Customer::getOrderBuilder() {
    return dynamic_cast<ConcreteOrderBuilder*>(orderBuilder);
}

// ============= Observer Pattern Implementation (Pure Pattern) =============

// Override from CustomerSubject - notifies all observers
void Customer::notifyInteraction(const std::string& interactionType, const std::string& details) {
    std::cout << "[CUSTOMER NOTIFICATION] " << name << " - " << interactionType << std::endl;
    if (!details.empty()) {
        std::cout << "   Details: " << details << std::endl;
    }
    
    // Notify all attached observers
    for (CustomerObserver* observer : observers) {
        if (observer) {
            observer->updateCustomerInteraction(this, interactionType, details);
        }
    }
}

// Override from CustomerSubject - requests validation from observers
bool Customer::requestValidation(Order* order) {
    if (observers.empty()) {
        std::cout << "[ERROR] No staff observers available for order validation" << std::endl;
        return false;
    }
    
    std::cout << "\n[VALIDATION REQUEST] Sending to staff observers..." << std::endl;
    std::cout << "Customer: " << name << " (" << email << ")" << std::endl;
    std::cout << "Order ID: " << order->getOrderId() << std::endl;
    
    // Try each observer until one validates successfully
    for (CustomerObserver* observer : observers) {
        if (observer && observer->validateCustomerOrder(order, this)) {
            std::cout << "[SUCCESS] Order validated by staff" << std::endl;
            return true;
        }
    }
    
    std::cout << "[FAILED] No staff could validate the order" << std::endl;
    return false;
}

// Convenience methods for attaching/detaching observers
void Customer::attachObserver(CustomerObserver* observer) {
    attach(observer);
    std::cout << "[SYSTEM] Staff observer registered for customer: " << name << std::endl;
}

void Customer::detachObserver(CustomerObserver* observer) {
    detach(observer);
    std::cout << "[SYSTEM] Staff observer unregistered for customer: " << name << std::endl;
}

// Helper methods
std::vector<PlantProduct*> Customer::getAvailablePlantsFromInventory() {
    InventoryManager& inventory = InventoryManager::getInstance();
    return inventory.getReadyForSalePlants();
}

void Customer::displayPlantDetails(const PlantProduct* plant, int index) {
    std::cout << std::setw(3) << index << ". ";
    
    if (plant && plant->getProfile()) {
        std::cout << std::setw(15) << plant->getProfile()->getSpeciesName();
        std::cout << " | State: " << std::setw(12) << plant->getCurrentStateName();
        
        // Display plant care requirements if available
        std::string waterNeeds = plant->getProfile()->getProperty("idealWater");
        std::string sunNeeds = plant->getProfile()->getProperty("idealSunlight");
        
        if (!waterNeeds.empty()) {
            std::cout << " | Water: " << waterNeeds;
        }
        if (!sunNeeds.empty()) {
            std::cout << " | Sun: " << sunNeeds;
        }
        
        std::cout << " | $15.99"; // Default price - could be enhanced
    } else {
        std::cout << "Unknown Plant";
    }
    std::cout << std::endl;
}

// Director-based construction methods

Order* Customer::construct() {
    std::cout << "\n=== Using Director to construct default order ===" << std::endl;
    
    // Clean up any previous order
    delete orderProduct;
    orderProduct = 0;
    
    // Notify observers about order construction
    notifyInteraction("ORDER_CONSTRUCTION_STARTED", "Customer initiated order construction via Director");
    
    // Use the director to construct the order
    orderProduct = orderDirector->construct();
    
    if (orderProduct) {
        std::cout << "Order successfully constructed via Director!" << std::endl;
        notifyInteraction("ORDER_CONSTRUCTED", "Director successfully built order");
    } else {
        std::cout << "Failed to construct order via Director." << std::endl;
        notifyInteraction("ORDER_CONSTRUCTION_FAILED", "Director failed to build order");
    }
    
    return orderProduct;
}

Order* Customer::constructSimplePlantOrder(const std::string& plantType, int quantity) {
    std::cout << "\n=== Constructing Simple Plant Order ===" << std::endl;
    std::cout << "Plant: " << plantType << ", Quantity: " << quantity << std::endl;
    
    // Clean up any previous order
    delete orderProduct;
    orderProduct = 0;
    
    // Notify observers
    notifyInteraction("SIMPLE_PLANT_ORDER", "Constructing simple plant order: " + plantType);
    
    // Use director to construct
    orderProduct = orderDirector->constructSimplePlantOrder(plantType, quantity);
    
    if (orderProduct) {
        std::cout << "Simple plant order constructed successfully!" << std::endl;
    }
    
    return orderProduct;
}

Order* Customer::constructPlantWithPotOrder(const std::string& plantType, const std::string& potType, int quantity) {
    std::cout << "\n=== Constructing Plant with Pot Order ===" << std::endl;
    std::cout << "Plant: " << plantType << ", Pot: " << potType << ", Quantity: " << quantity << std::endl;
    
    // Clean up any previous order
    delete orderProduct;
    orderProduct = 0;
    
    // Notify observers
    notifyInteraction("PLANT_POT_ORDER", "Constructing plant+pot order: " + plantType + " + " + potType);
    
    // Use director to construct
    orderProduct = orderDirector->constructPlantWithPotOrder(plantType, potType, quantity);
    
    if (orderProduct) {
        std::cout << "Plant with pot order constructed successfully!" << std::endl;
    }
    
    return orderProduct;
}

Order* Customer::constructBundleOrder(const std::string& bundleName, 
                                     const std::vector<std::string>& plantTypes,
                                     const std::vector<int>& quantities, 
                                     double discount) {
    std::cout << "\n=== Constructing Bundle Order ===" << std::endl;
    std::cout << "Bundle: " << bundleName << ", Discount: " << discount << "%" << std::endl;
    
    // Clean up any previous order
    delete orderProduct;
    orderProduct = 0;
    
    // Notify observers
    notifyInteraction("BUNDLE_ORDER", "Constructing bundle order: " + bundleName);
    
    // Use director to construct
    orderProduct = orderDirector->constructBundleOrder(bundleName, plantTypes, quantities, discount);
    
    if (orderProduct) {
        std::cout << "Bundle order constructed successfully!" << std::endl;
    }
    
    return orderProduct;
}