#include "Customer.h"
#include "ConcreteOrderBuilder.h"
#include "PlaceOrderCommand.h"
#include "Order.h"
#include "InventoryManager.h"
#include "PlantProduct.h"
#include "StaffManager.h"
#include "BridgeDP/PlantSpeciesProfile.h"
#include "OrderHistory.h"
#include <iostream>
#include <iomanip>

#include "PaymentProcessor.h"
#include "CashAdapter.h"
#include "CreditCardAdapter.h"
#include "EFTAdapter.h"
#include "CashAdaptee.h"
#include "CreditCardAdaptee.h"
#include "EFTAdaptee.h"

#include "SuggestionTemplate/BouquetSuggestionFactory.h"

#include <vector>
#include <map>
#include <string>

using namespace std;

Customer::Customer(const std::string& name, const std::string& email, const std::string& cellPhone)
    : name(name), email(email), cellPhone(cellPhone), orderBuilder(nullptr), orderProduct(nullptr), placeOrderCommand(nullptr), staffObserver(nullptr) {
    // Initialize the order builder
    orderBuilder = new ConcreteOrderBuilder(name);
    orderHistory = new OrderHistory();

    // Initialize payment systems
    initializePaymentSystems();
}

Customer::~Customer() {
    delete orderBuilder;
    delete orderProduct;
    delete placeOrderCommand;
    delete orderHistory;

    // Cleanup payment systems
    cleanupPaymentSystems();
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
        std::cout << "[TOTAL] Total: R" << std::fixed << std::setprecision(2) 
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
    std::cout << "[TOTAL] Final Total: R" << std::fixed << std::setprecision(2) 
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
        
        std::cout << " | R15.99"; // Default price - could be enhanced
    } else {
        std::cout << "Unknown Plant";
    }
    std::cout << std::endl;
}

// Memento pattern - order history methods
void Customer::saveCurrentOrder() {
    // First, get or create the current order from builder
    ConcreteOrderBuilder* concreteBuilder = dynamic_cast<ConcreteOrderBuilder*>(orderBuilder);
    if (!concreteBuilder || !concreteBuilder->hasCurrentOrder()) {
        std::cout << "[MEMENTO] No order to save - order not built yet" << std::endl;
        return;
    }
    
    // Get the current order if we don't have it yet (DON'T delete it first)
    if (!orderProduct) {
        orderProduct = orderBuilder->getOrder();
    }
    
    if (orderProduct && !orderProduct->isEmpty()) {
        orderHistory->saveOrder(orderProduct);
        std::cout << "[MEMENTO] Current order saved to history (Item count: " 
                  << orderProduct->getItemCount() << ")" << std::endl;
    } else {
        std::cout << "[MEMENTO] No order to save - order is empty" << std::endl;
    }
}

void Customer::restoreLastOrder() {
    if (orderProduct) {
        orderHistory->undo(orderProduct);
        std::cout << "[MEMENTO] Last order state restored from history" << std::endl;
        viewCurrentOrder();
    } else {
        std::cout << "[MEMENTO] No order to restore" << std::endl;
    }
}

void Customer::viewOrderHistory() {
    std::cout << "\n=== ORDER HISTORY ===" << std::endl;
    std::cout << "You can restore previous order states using restoreLastOrder()" << std::endl;
    std::cout << "Current order: " << std::endl;
    viewCurrentOrder();
}
// Adapter pattern - payment processing //
void Customer::initializePaymentSystems()
{
    std::cout << "[Payment] Initializing payment adapters for customer..." << std::endl;
    
    // Create adaptees (third-party payment systems)
    cashSystem = new CashAdaptee();
    creditCardSystem = new CreditCardAdaptee();
    eftSystem = new EFTAdaptee();
    
    // Create adapters and register them
    paymentAdapters["CASH"] = new CashAdapter(cashSystem);
    paymentAdapters["CREDIT_CARD"] = new CreditCardAdapter(creditCardSystem);
    paymentAdapters["EFT"] = new EFTAdapter(eftSystem);
    
    std::cout << "[Payment] Registered payment methods: ";
    for (const auto& pair : paymentAdapters) {
        std::cout << pair.first << " ";
    }
    std::cout << std::endl;
}

void Customer::cleanupPaymentSystems() {
    // Delete adapters
    for (auto& pair : paymentAdapters) {
        delete pair.second;
    }
    paymentAdapters.clear();
    
    // Delete adaptees
    delete cashSystem;
    delete creditCardSystem;
    delete eftSystem;
}

bool Customer::processPayment(const std::string& paymentType, double amount, 
                              const std::string& paymentDetails) {
    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║     PROCESSING PAYMENT                ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;
    
    std::cout << "Customer: " << name << " (" << email << ")" << std::endl;
    std::cout << "Payment Type: " << paymentType << std::endl;
    std::cout << "Amount: R" << std::fixed << std::setprecision(2) << amount << std::endl;
    
    // Find the appropriate adapter
    auto it = paymentAdapters.find(paymentType);
    if (it == paymentAdapters.end()) {
        std::cout << "\n[ERROR] Unsupported payment type: " << paymentType << std::endl;
        std::cout << "Available methods: ";
        for (const auto& pair : paymentAdapters) {
            std::cout << pair.first << " ";
        }
        std::cout << std::endl;
        return false;
    }

    // Process payment through the adapter
    std::cout << "\n[Processing] Using " << paymentType << " adapter..." << std::endl;
    bool success = it->second->processPayment(amount, email, paymentDetails);
    
    if (success) {
        std::cout << "\n✓ Payment processed successfully!" << std::endl;
        std::cout << "Transaction completed for " << name << std::endl;
    } else {
        std::cout << "\n✗ Payment failed!" << std::endl;
        std::cout << "Please check your payment details and try again." << std::endl;
    }
    
    return success;
}

bool Customer::isPaymentMethodSupported(const std::string& paymentType) const {
    bool supported = paymentAdapters.find(paymentType) != paymentAdapters.end();
    std::cout << "[DEBUG] Checking support for '" << paymentType << "': " 
              << (supported ? "YES" : "NO") << std::endl;
    return supported;
}

bool Customer::executeOrderWithPayment(const std::string& paymentType, 
                                       const std::string& paymentDetails) {
    if (!orderProduct || orderProduct->isEmpty()) {
        std::cout << "[ERROR] No order to execute. Please finalize your order first." << std::endl;
        return false;
    }
    
    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║   INTEGRATED ORDER EXECUTION          ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝\n" << std::endl;
    
    // Step 1: Save order state (Memento pattern)
    std::cout << "[Step 1] Saving order state before processing..." << std::endl;
    saveCurrentOrder();
    
    // Step 2: Display order summary
    std::cout << "\n[Step 2] Order Summary:" << std::endl;
    std::cout << orderProduct->getOrderSummary() << std::endl;
    
    // Step 3: Validate payment method
    std::cout << "\n[Step 3] Validating payment method..." << std::endl;
    if (!isPaymentMethodSupported(paymentType)) {
        std::cout << "[ERROR] Payment method '" << paymentType << "' is not supported." << std::endl;
        showPaymentOptions();
        return false;
    }
    std::cout << "[OK] Payment method '" << paymentType << "' is supported." << std::endl;
    
    // Step 4: Request staff validation
    std::cout << "\n[Step 4] Requesting staff validation..." << std::endl;
    if (!requestStaffValidation(orderProduct)) {
        std::cout << "[ERROR] Staff validation failed." << std::endl;
        std::cout << "Restoring order to previous state..." << std::endl;
        restoreLastOrder();
        return false;
    }
    std::cout << "[OK] Staff validation completed." << std::endl;
    
    // Step 5: Process payment (Adapter pattern)
    std::cout << "\n[Step 5] Processing payment..." << std::endl;
    double totalAmount = orderProduct->getTotalAmount();
    bool paymentSuccess = processPayment(paymentType, totalAmount, paymentDetails);
    
    if (!paymentSuccess) {
        std::cout << "\n[ERROR] Payment processing failed." << std::endl;
        std::cout << "Order has NOT been completed." << std::endl;
        std::cout << "You can try again with a different payment method." << std::endl;
        return false;
    }
    
    // Step 6: Finalize order
    std::cout << "\n[Step 6] Finalizing order..." << std::endl;
    orderProduct->setStatus("Completed - Paid");
    
    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║     ORDER COMPLETED SUCCESSFULLY!     ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;
    
    std::cout << "\nOrder ID: " << orderProduct->getOrderId() << std::endl;
    std::cout << "Status: " << orderProduct->getStatus() << std::endl;
    std::cout << "Total Paid: R" << std::fixed << std::setprecision(2) << totalAmount << std::endl;
    std::cout << "\nThank you for your purchase, " << name << "!" << std::endl;
    
    return true;
}

void Customer::showPaymentOptions() const {
    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║   AVAILABLE PAYMENT METHODS           ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;
    
    std::cout << "\n1. CASH" << std::endl;
    std::cout << "   - Pay with cash" << std::endl;
    std::cout << "   - No additional details required" << std::endl;
    std::cout << "   - Usage: processPayment(\"CASH\", amount, \"\")" << std::endl;
    
    std::cout << "\n2. CREDIT_CARD" << std::endl;
    std::cout << "   - Pay with credit/debit card" << std::endl;
    std::cout << "   - Format: \"cardNumber;expiry;cvc\"" << std::endl;
    std::cout << "   - Example: \"4532123456789012;12/25;123\"" << std::endl;
    std::cout << "   - Usage: processPayment(\"CREDIT_CARD\", amount, cardDetails)" << std::endl;
    
    std::cout << "\n3. EFT" << std::endl;
    std::cout << "   - Electronic Funds Transfer" << std::endl;
    std::cout << "   - Direct bank transfer" << std::endl;
    std::cout << "   - Usage: processPayment(\"EFT\", amount, \"EFT\")" << std::endl;
    
    std::cout << "\n" << std::string(44, '=') << std::endl;
}

//suggested Bouqeut

void Customer::browseBouquetSuggestions(const std::string& eventType) {
    BouquetSuggestionFactory& factory = BouquetSuggestionFactory::getInstance();
    BouquetSuggestionTemplate* tmpl = factory.getTemplate(eventType);
    
    if (!tmpl) {
        std::cout << "Sorry, we don't have suggestions for that event yet." << std::endl;
        return;
    }
    
    // The template method handles the entire algorithm!
    std::vector<BouquetSuggestion> suggestions = tmpl->generateSuggestions();
    
    // Display suggestions
    for (size_t i = 0; i < suggestions.size(); i++) {
        std::cout << "\n[Option " << (i+1) << "]" << std::endl;
        std::cout << suggestions[i].getDescription() << std::endl;
        std::cout << std::string(60, '-') << std::endl;
    }
}