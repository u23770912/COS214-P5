#include "Customer.h"
#include "ConcreteOrderBuilder.h"
#include "OrderDirector.h"
#include "OrderUIFacade.h"
#include "PlaceOrderCommand.h"
#include "Order.h"
#include "InventoryManager.h"
#include "PlantProduct.h"
#include "CustomerObserver.h"
#include "BridgeDP/PlantSpeciesProfile.h"
#include <iostream>
#include <iomanip>

Customer::Customer(const std::string& name, const std::string& email, const std::string& cellPhone)
    : name(name), email(email), cellPhone(cellPhone), orderBuilder(0), orderDirector(0), uiFacade(0), orderProduct(0), placeOrderCommand(0) {
    // Initialize the order builder - Customer's core responsibility
    orderBuilder = new ConcreteOrderBuilder(name);
    
    // Initialize the order director with the builder
    orderDirector = new OrderDirector(orderBuilder);
    
    // Initialize UI facade - Separates UI concerns from business logic
    uiFacade = new OrderUIFacade(this);
}

Customer::~Customer() {
    delete uiFacade;
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
    // Customer focuses only on order creation logic, UI handled by facade
    cleanupPreviousOrder();
    
    // Reset the builder for a new order
    orderBuilder->reset();
    
    // Use facade to display welcome message and instructions
    uiFacade->displayOrderCreationWelcome();
    
    return 0; // Order will be created when finalized
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
    
    // Use facade to display execution summary
    uiFacade->displayOrderExecutionSummary();
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

// UI operations delegated to facade - maintaining compatibility for existing tests
void Customer::displayAvailableItems() {
    uiFacade->displayAvailableItems();
}

void Customer::viewCurrentOrder() {
    uiFacade->viewCurrentOrder();
}

bool Customer::addPlantToOrder(int plantIndex, int quantity) {
    // Delegate to facade which handles UI and automatic discount logic
    return uiFacade->addPlantToOrderWithAutoDiscount(plantIndex, quantity);
}

bool Customer::addBundleToOrder(const std::string& bundleName, const std::vector<int>& plantIndices) {
    // Delegate to facade which handles validation, UI, and automatic discount logic
    return uiFacade->addBundleToOrderWithAutoDiscount(bundleName, plantIndices);
}

bool Customer::finalizeOrder() {
    ConcreteOrderBuilder* concreteBuilder = dynamic_cast<ConcreteOrderBuilder*>(orderBuilder);
    if (!concreteBuilder || !concreteBuilder->hasCurrentOrder()) {
        std::cout << "[ERROR] No order to finalize. Add some plants first!" << std::endl;
        return false;
    }
    
    // Core business logic: finalize the order
    delete orderProduct;
    orderProduct = orderBuilder->getOrder();
    
    if (!orderProduct || orderProduct->isEmpty()) {
        std::cout << "[ERROR] Cannot finalize empty order." << std::endl;
        return false;
    }
    
    // Use facade to display finalization confirmation
    std::cout << orderProduct->getOrderSummary() << std::endl;
    std::cout << "[TOTAL] Final Total: $" << std::fixed << std::setprecision(2) 
              << orderProduct->getTotalAmount() << std::endl;
    uiFacade->displayFinalOrderConfirmation();
    
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

// Helper methods for facade integration
OrderUIFacade* Customer::getUIFacade() {
    return uiFacade;
}

void Customer::cleanupPreviousOrder() {
    if (orderProduct) {
        delete orderProduct;
        orderProduct = 0;
    }
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