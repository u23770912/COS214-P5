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
#include "OrderHistory.h"
#include "StaffManager.h"
#include <iostream>
#include <iomanip>

#include "PaymentProcessor.h"
#include "CashAdapter.h"
#include "CreditCardAdapter.h"
#include "EFTAdapter.h"
#include "CashAdaptee.h"
#include "CreditCardAdaptee.h"
#include "EFTAdaptee.h"

#include <vector>
#include <map>

Customer::Customer(const std::string& name, const std::string& email, const std::string& cellPhone)
    : name(name), email(email), cellPhone(cellPhone), orderBuilder(0), orderDirector(0), uiFacade(0), orderProduct(0), placeOrderCommand(0), 
      staffObserver(0), orderHistory(0), cashSystem(0), creditCardSystem(0), eftSystem(0) {
    // Initialize the order builder - Customer's core responsibility
    orderBuilder = new ConcreteOrderBuilder(name);
    
    // Initialize the order director with the builder
    orderDirector = new OrderDirector(orderBuilder);
    
    // Initialize UI facade - Separates UI concerns from business logic
    uiFacade = new OrderUIFacade(this);
    
    // Initialize order history (Memento pattern)
    orderHistory = new OrderHistory();
    
    // Initialize payment systems (Adapter pattern)
    initializePaymentSystems();
}

Customer::~Customer() {
    delete uiFacade;
    delete orderDirector;
    delete orderBuilder;
    delete orderProduct;
    delete placeOrderCommand;
    delete orderHistory;
    
    // Clean up payment systems
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

// ============= Additional Methods from Sales Floor Integration =============

// Memento pattern - order history methods
void Customer::saveCurrentOrder() {
    if(orderProduct && !orderProduct->isEmpty()) {
        orderHistory->saveOrder(orderProduct);
        std::cout << "[SAVED] Current order saved to history" << std::endl;
    } else {
        std::cout << "[ERROR] No order to save" << std::endl;
    }
}

void Customer::restoreLastOrder()
{
    if(orderProduct)
    {
        orderHistory->undo(orderProduct);
        std::cout << "[RESTORED] Last order state restored from history" << std::endl;
        viewCurrentOrder();
    }
    else
    {
        std::cout << "[ERROR] No order to restore" << std::endl;
    }
}

void Customer::viewOrderHistory()
{
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
    
    std::cout << "[Payment] Available payment methods: CASH, CREDIT_CARD, EFT" << std::endl;
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
        std::cout << "Available methods: CASH, CREDIT_CARD, EFT" << std::endl;
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
    return paymentAdapters.find(paymentType) != paymentAdapters.end();
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

// ============= Additional Staff Observer Methods (from salesfloor) =============

void Customer::setStaffObserver(StaffManager* staff) {
    staffObserver = staff;
    if (staff) {
        std::cout << "[SYSTEM] Staff observer set for customer: " << name << std::endl;
    }
}

void Customer::notifyStaffOfInteraction(const std::string& interactionType, const std::string& details) {
    if (staffObserver) {
        std::cout << "[STAFF NOTIFICATION] " << name << " - " << interactionType << std::endl;
        if (!details.empty()) {
            std::cout << "   Details: " << details << std::endl;
        }
        staffObserver->updateCustomerInteraction(this, interactionType, details);
    } else {
        std::cout << "[WARNING] No staff observer set for customer: " << name << std::endl;
    }
}

bool Customer::requestStaffValidation(Order* order) {
    if (!staffObserver) {
        std::cout << "[ERROR] No staff observer available for order validation" << std::endl;
        return false;
    }
    
    std::cout << "\n[STAFF VALIDATION REQUEST] Requesting validation from staff observer..." << std::endl;
    std::cout << "Customer: " << name << " (" << email << ")" << std::endl;
    std::cout << "Order ID: " << order->getOrderId() << std::endl;
    
    if (staffObserver->validateCustomerOrder(order, this)) {
        std::cout << "[SUCCESS] Order validated by staff observer" << std::endl;
        return true;
    }
    
    std::cout << "[FAILED] Staff observer could not validate the order" << std::endl;
    return false;
}