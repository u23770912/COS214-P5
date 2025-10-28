#include <iostream>
#include "Customer.h"
#include "StaffManager.h"
#include "StaffMember.h"
#include "Order.h"
#include "ConcreteOrderBuilder.h"
#include "InventoryManager.h"
#include "PlantProduct.h"
#include "BridgeDP/SucculentProfile.h"
#include "BridgeDP/FlowerProfile.h"
#include "StateDP/ReadyForSaleState.h"

using namespace std;

/**
 * @brief Demonstrates the Pure Observer Pattern for Customer-Staff interactions
 * 
 * Pattern Structure:
 * - Subject (abstract): CustomerSubject
 * - ConcreteSubject: Customer
 * - Observer (abstract): CustomerObserver
 * - ConcreteObserver: StaffManager
 */

int main() {
    cout << "\n╔════════════════════════════════════════════════════════════╗" << endl;
    cout << "║  PURE OBSERVER PATTERN DEMONSTRATION                      ║" << endl;
    cout << "║  Customer (ConcreteSubject) → StaffManager (ConcreteObserver) ║" << endl;
    cout << "╚════════════════════════════════════════════════════════════╝\n" << endl;
    
    // Setup inventory
    cout << "=== Setting up Inventory ===" << endl;
    InventoryManager& inventory = InventoryManager::getInstance();
    
    PlantProduct* rose = new PlantProduct("R001", new FlowerProfile("Rose", "moderate", "full sun", "loamy"));
    rose->transitionTo(new ReadyForSaleState());
    inventory.moveToSalesFloor(rose);
    
    PlantProduct* succulent = new PlantProduct("S001", new SucculentProfile("Succulent", "low", "bright", "sandy"));
    succulent->transitionTo(new ReadyForSaleState());
    inventory.moveToSalesFloor(succulent);
    
    cout << "✓ Inventory populated\n" << endl;
    
    // Setup Observer Pattern Participants
    cout << "=== Setting up Observer Pattern Participants ===" << endl;
    
    // Create ConcreteObserver (StaffManager)
    StaffMember* staffDispatcher = new StaffMember();
    StaffManager* staffManager = new StaffManager(staffDispatcher);
    cout << "✓ Created ConcreteObserver: StaffManager" << endl;
    
    // Create ConcreteSubject (Customer)
    Customer* customer = new Customer("John Doe", "john@example.com", "555-1234");
    cout << "✓ Created ConcreteSubject: Customer" << endl;
    
    // Attach Observer to Subject (Pure Observer Pattern!)
    cout << "\n--- ATTACHING OBSERVER TO SUBJECT ---" << endl;
    customer->attachObserver(staffManager);
    
    cout << "\n╔════════════════════════════════════════════════════════════╗" << endl;
    cout << "║  DEMONSTRATING OBSERVER NOTIFICATIONS                     ║" << endl;
    cout << "╚════════════════════════════════════════════════════════════╝\n" << endl;
    
    // Test 1: Customer selects plants (triggers notification)
    cout << "=== TEST 1: Customer Plant Selection ===" << endl;
    customer->notifyInteraction("PlantSelection", "Customer browsing Rose plants");
    
    cout << "\n=== TEST 2: Customer Creates Bundle ===" << endl;
    customer->notifyInteraction("BundleCreation", "Creating Garden Starter Bundle");
    
    cout << "\n=== TEST 3: Customer Requests Assistance ===" << endl;
    customer->notifyInteraction("AssistanceNeeded", "Question about plant care");
    
    // Test 4: Order Validation (Observer validates the order)
    cout << "\n\n╔════════════════════════════════════════════════════════════╗" << endl;
    cout << "║  DEMONSTRATING ORDER VALIDATION THROUGH OBSERVER          ║" << endl;
    cout << "╚════════════════════════════════════════════════════════════╝\n" << endl;
    
    // Build a sample order
    ConcreteOrderBuilder* builder = customer->getOrderBuilder();
    builder->buildPlant("Rose", 2);
    builder->buildPlant("Succulent", 1);
    
    Order* order = builder->getOrder();
    
    cout << "=== TEST 4: Order Validation Request ===" << endl;
    cout << "Customer submitting order for staff validation...\n" << endl;
    
    // Request validation from observers (Pure Observer Pattern!)
    bool validated = customer->requestValidation(order);
    
    if (validated) {
        cout << "\n[SUCCESS] Order was validated by staff observers!" << endl;
    } else {
        cout << "\n[FAILED] Order validation failed!" << endl;
    }
    
    // Test 5: Detach observer
    cout << "\n\n=== TEST 5: Detaching Observer ===" << endl;
    customer->detachObserver(staffManager);
    
    cout << "\nAttempting notification after detaching observer..." << endl;
    customer->notifyInteraction("PlantSelection", "This should not reach staff");
    
    // Summary
    cout << "\n\n╔════════════════════════════════════════════════════════════╗" << endl;
    cout << "║  PURE OBSERVER PATTERN SUMMARY                            ║" << endl;
    cout << "╠════════════════════════════════════════════════════════════╣" << endl;
    cout << "║  Subject (Abstract):     CustomerSubject                  ║" << endl;
    cout << "║  ConcreteSubject:        Customer                         ║" << endl;
    cout << "║  Observer (Abstract):    CustomerObserver                 ║" << endl;
    cout << "║  ConcreteObserver:       StaffManager                     ║" << endl;
    cout << "║                                                            ║" << endl;
    cout << "║  Key Methods:                                             ║" << endl;
    cout << "║  - attach(observer)       → Register observer             ║" << endl;
    cout << "║  - detach(observer)       → Unregister observer           ║" << endl;
    cout << "║  - notifyInteraction()    → Notify all observers          ║" << endl;
    cout << "║  - updateCustomerInteraction() → Observer receives update ║" << endl;
    cout << "║  - validateCustomerOrder() → Observer validates order     ║" << endl;
    cout << "╚════════════════════════════════════════════════════════════╝" << endl;
    
    // Cleanup
    delete customer;
    delete staffManager;
    delete staffDispatcher;
    
    cout << "\n[DEMO COMPLETE]" << endl;
    return 0;
}
