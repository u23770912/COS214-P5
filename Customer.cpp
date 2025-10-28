#include "Customer.h"
#include "ConcreteOrderBuilder.h"
#include "PlaceOrderCommand.h"
#include "Order.h"
#include "InventoryManager.h"
#include "PlantProduct.h"
#include "StaffManager.h"
#include "BridgeDP/PlantSpeciesProfile.h"
#include <iostream>
#include <iomanip>

Customer::Customer(const std::string& name, const std::string& email, const std::string& cellPhone)
    : name(name), email(email), cellPhone(cellPhone), orderBuilder(nullptr), orderProduct(nullptr), placeOrderCommand(nullptr), staffObserver(nullptr) {
    // Initialize the order builder
    orderBuilder = new ConcreteOrderBuilder(name);
}

Customer::~Customer() {
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
    if (!requestStaffValidation(orderProduct)) {
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
    
    // Notify staff of customer interaction
    notifyStaffOfInteraction("PlantSelection", 
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
    
    // Notify staff of bundle creation
    notifyStaffOfInteraction("BundleCreation", 
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

// Staff interaction methods - Observer pattern implementation
void Customer::setStaffObserver(StaffManager* staff) {
    staffObserver = staff;
    std::cout << "[SYSTEM] Customer " << name << " connected to staff system" << std::endl;
}

void Customer::notifyStaffOfInteraction(const std::string& interactionType, const std::string& details) {
    if (staffObserver) {
        std::cout << "[STAFF] Customer " << name << " requesting staff assistance: " << interactionType << std::endl;
        if (!details.empty()) {
            std::cout << "   Details: " << details << std::endl;
        }
        // Notify staff through observer pattern
        // staffObserver->handleCustomerInteraction(this, interactionType, details);
    } else {
        std::cout << "[WARNING] No staff available - customer service request ignored" << std::endl;
    }
}

bool Customer::requestStaffValidation(Order* order) {
    if (!staffObserver) {
        std::cout << "[ERROR] No staff available for order validation" << std::endl;
        return false;
    }
    
    std::cout << "\n[STAFF REQUEST] REQUESTING STAFF VALIDATION" << std::endl;
    std::cout << "Customer: " << name << " (" << email << ")" << std::endl;
    std::cout << "Order ID: " << order->getOrderId() << std::endl;
    
    notifyStaffOfInteraction("OrderValidation", "Customer needs order validation for " + order->getOrderId());
    
    // In a real system, this would be asynchronous
    // For demo, we'll simulate staff processing
    std::cout << "[PROCESSING] Staff processing validation request..." << std::endl;
    
    // Simulate staff validation through chain
    return true; // For now, always succeed
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