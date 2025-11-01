#include <iostream>
#include <vector>
#include "Customer.h"
#include "Order.h"
#include "ConcreteOrderBuilder.h"
#include "SinglePlant.h"
#include "PlantBundle.h"
#include "InventoryManager.h"
#include "PlantProduct.h"
#include "BridgeDP/SucculentProfile.h"
#include "BridgeDP/FlowerProfile.h"
#include "BridgeDP/TreeProfile.h"
#include "StateDP/ReadyForSaleState.h"
#include "Cashier.h"
#include "StaffMember.h"
#include "OrderValidationHandler.h"
#include "PaymentProcessHandler.h"
#include "NotificationHandler.h"
#include "OrderProcessHandler.h"
#include "Command.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

// Helper function to populate inventory with sample plants
void populateInventory() {
    InventoryManager& inventory = InventoryManager::getInstance();
    
    cout << "\n=== Setting up Sales Floor Inventory ===" << endl;
    
    // Add various plants to sales floor
    PlantProduct* aloe1 = new PlantProduct("A001", new SucculentProfile("Aloe Vera", "low", "bright", "sandy"));
    aloe1->transitionTo(new ReadyForSaleState());
    inventory.moveToSalesFloor(aloe1);
    
    PlantProduct* aloe2 = new PlantProduct("A002", new SucculentProfile("Aloe Vera", "low", "bright", "sandy"));
    aloe2->transitionTo(new ReadyForSaleState());
    inventory.moveToSalesFloor(aloe2);
    
    PlantProduct* rose1 = new PlantProduct("R001", new FlowerProfile("Rose", "moderate", "full sun", "loamy"));
    rose1->transitionTo(new ReadyForSaleState());
    inventory.moveToSalesFloor(rose1);
    
    PlantProduct* rose2 = new PlantProduct("R002", new FlowerProfile("Rose", "moderate", "full sun", "loamy"));
    rose2->transitionTo(new ReadyForSaleState());
    inventory.moveToSalesFloor(rose2);
    
    PlantProduct* oak1 = new PlantProduct("O001", new TreeProfile("Oak", "moderate", "full sun", "well-drained"));
    oak1->transitionTo(new ReadyForSaleState());
    inventory.moveToSalesFloor(oak1);
    
    cout << "Inventory populated with " << inventory.getReadyForSalePlants().size() << " plants" << endl;
    cout << "=========================================\n" << endl;
}

// Helper function to display menu
void displayMenu() {
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║     CUSTOMER ORDER MENU               ║" << endl;
    cout << "╠════════════════════════════════════════╣" << endl;
    cout << "║ 1. View Available Plants              ║" << endl;
    cout << "║ 2. Add Single Plant to Order          ║" << endl;
    cout << "║ 3. Add Plant Bundle to Order          ║" << endl;
    cout << "║ 4. View Current Order                 ║" << endl;
    cout << "║ 5. Submit Order for Validation        ║" << endl;
    cout << "║ 6. Exit                               ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    cout << "Enter your choice: ";
}

void displayAvailablePlants() {
    InventoryManager& inventory = InventoryManager::getInstance();
    auto plants = inventory.getReadyForSalePlants();
    
    cout << "\n┌────────────────────────────────────────┐" << endl;
    cout << "│     AVAILABLE PLANTS FOR SALE         │" << endl;
    cout << "├────────────────────────────────────────┤" << endl;
    
    if (plants.empty()) {
        cout << "│  No plants available                  │" << endl;
    } else {
        for (size_t i = 0; i < plants.size(); i++) {
            cout << "│ " << i+1 << ". " << plants[i]->getProfile()->getSpeciesName();
            
            // Pad to align
            string name = plants[i]->getProfile()->getSpeciesName();
            for (size_t j = name.length(); j < 30; j++) cout << " ";
            cout << " │" << endl;
        }
    }
    cout << "└────────────────────────────────────────┘" << endl;
}

int main() {
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║   GREENHOUSE CUSTOMER ORDER SYSTEM    ║" << endl;
    cout << "╚════════════════════════════════════════╝\n" << endl;
    
    // Setup inventory
    populateInventory();
    
    // Setup cashier staff chain
    cout << "=== Setting up Sales Staff ===" << endl;
    SalesFloorStaff* cashier1 = new Cashier();
    SalesFloorStaff* cashier2 = new Cashier();
    cashier1->setNext(cashier2);
    
    StaffMember* dispatcher = new StaffMember();
    dispatcher->registerTeam("Sales", cashier1);
    cout << "Sales team configured with 2 cashiers" << endl;
    cout << "==================================\n" << endl;
    
    // Create customer
    cout << "=== Customer Information ===" << endl;
    string name, email, phone;
    cout << "Enter your name: ";
    std::getline(cin, name);
    cout << "Enter your email: ";
    std::getline(cin, email);
    cout << "Enter your phone: ";
    std::getline(cin, phone);
    
    Customer* customer = new Customer(name, email, phone);
    cout << "\nWelcome, " << name << "!" << endl;
    cout << "===========================\n" << endl;
    
    // Create order builder
    ConcreteOrderBuilder* orderBuilder = new ConcreteOrderBuilder(name);
    Order* currentOrder = nullptr;
    
    bool running = true;
    while (running) {
        displayMenu();
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1: {
                displayAvailablePlants();
                break;
            }
            
            case 2: {
                // Add single plant
                displayAvailablePlants();
                InventoryManager& inventory = InventoryManager::getInstance();
                auto plants = inventory.getReadyForSalePlants();
                
                if (plants.empty()) {
                    cout << "\nNo plants available!" << endl;
                    break;
                }
                
                cout << "\nEnter plant number to add (1-" << plants.size() << "): ";
                int plantNum;
                cin >> plantNum;
                
                if (plantNum < 1 || plantNum > (int)plants.size()) {
                    cout << "Invalid plant number!" << endl;
                    break;
                }
                
                cout << "Enter quantity: ";
                int quantity;
                cin >> quantity;
                
                // Create order if it doesn't exist
                if (!currentOrder) {
                    currentOrder = orderBuilder->getOrder();
                }
                
                // Add plant to order
                string plantType = plants[plantNum-1]->getProfile()->getSpeciesName();
                SinglePlant* plant = new SinglePlant(plantType, 25.99, quantity);
                currentOrder->addOrderItem(plant);
                
                cout << "\n✓ Added " << quantity << "x " << plantType << " to order" << endl;
                break;
            }
            
            case 3: {
                // Add plant bundle
                cout << "\n=== Create Plant Bundle ===" << endl;
                cout << "Enter bundle name: ";
                cin.ignore();
                string bundleName;
                std::getline(cin, bundleName);
                
                cout << "Enter discount percentage (0-50): ";
                double discount;
                cin >> discount;
                
                PlantBundle* bundle = new PlantBundle(bundleName, "Custom", 1, discount);
                
                // Add plants to bundle
                displayAvailablePlants();
                InventoryManager& inventory = InventoryManager::getInstance();
                auto plants = inventory.getReadyForSalePlants();
                
                if (plants.empty()) {
                    cout << "\nNo plants available for bundle!" << endl;
                    delete bundle;
                    break;
                }
                
                cout << "\nHow many different plants in bundle? ";
                int numPlants;
                cin >> numPlants;
                
                for (int i = 0; i < numPlants; i++) {
                    cout << "\nPlant " << (i+1) << " of " << numPlants << endl;
                    cout << "Enter plant number: ";
                    int plantNum;
                    cin >> plantNum;
                    
                    if (plantNum < 1 || plantNum > (int)plants.size()) {
                        cout << "Invalid plant number, skipping..." << endl;
                        continue;
                    }
                    
                    cout << "Enter quantity: ";
                    int qty;
                    cin >> qty;
                    
                    string plantType = plants[plantNum-1]->getProfile()->getSpeciesName();
                    SinglePlant* bundlePlant = new SinglePlant(plantType, 25.99, qty);
                    bundle->addItem(bundlePlant);
                }
                
                // Create order if it doesn't exist
                if (!currentOrder) {
                    currentOrder = orderBuilder->getOrder();
                }
                
                currentOrder->addOrderItem(bundle);
                cout << "\n✓ Bundle '" << bundleName << "' added to order!" << endl;
                break;
            }
            
            case 4: {
                // View current order
                if (!currentOrder || currentOrder->isEmpty()) {
                    cout << "\nYour order is empty" << endl;
                } else {
                    cout << "\n" << currentOrder->getOrderSummary() << endl;
                }
                break;
            }
            
            case 5: {
                // Submit order for validation
                if (!currentOrder || currentOrder->isEmpty()) {
                    cout << "\nCannot submit empty order!" << endl;
                    break;
                }
                
                cout << "\n╔════════════════════════════════════════╗" << endl;
                cout << "║   SUBMITTING ORDER FOR PROCESSING     ║" << endl;
                cout << "╚════════════════════════════════════════╝\n" << endl;
                
                cout << currentOrder->getOrderSummary() << endl;
                
                cout << "\n=== Order Processing Chain ===" << endl;
                cout << "Your order will go through:\n";
                cout << "1. Validation (Check inventory)\n";
                cout << "2. Payment Processing\n";
                cout << "3. Customer Notification\n" << endl;
                
                // Create handler chain
                OrderValidationHandler* validator = new OrderValidationHandler();
                PaymentProcessHandler* paymentProcessor = new PaymentProcessHandler();
                NotificationHandler* successNotifier = new NotificationHandler(false);
                NotificationHandler* failureNotifier = new NotificationHandler(true);
                
                // Set up success chain: Validation -> Payment -> Success Notification
                validator->setNext(paymentProcessor);
                paymentProcessor->setNext(successNotifier);
                
                cout << "=== Starting Order Processing ===" << endl;
                
                // Start the chain
                bool processingResult = validator->handleOrder(currentOrder, customer);
                
                if (!processingResult) {
                    // Validation or payment failed - send failure notification
                    cout << "\n=== Sending Failure Notification ===" << endl;
                    
                    // Get validation errors if validation failed
                    auto errors = validator->getValidationErrors();
                    if (!errors.empty()) {
                        failureNotifier->setErrorMessages(errors);
                    }
                    
                    failureNotifier->handleOrder(currentOrder, customer);
                    
                    cout << "\nORDER PROCESSING FAILED!" << endl;
                    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
                    cout << "A notification has been sent to your email with details." << endl;
                } else {
                    cout << "\nORDER PROCESSING COMPLETED SUCCESSFULLY!" << endl;
                    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
                    cout << "Order Status: " << currentOrder->getStatus() << endl;
                    cout << "Total Amount: R" << currentOrder->getTotalAmount() << endl;
                    cout << "\nA confirmation email has been sent to " << customer->getEmail() << endl;
                }
                
                // Cleanup handlers
                delete validator;
                delete paymentProcessor;
                delete successNotifier;
                delete failureNotifier;
                
                // Ask if user wants to continue
                cout << "\nCreate new order? (y/n): ";
                char continueChoice;
                cin >> continueChoice;
                if (continueChoice == 'n' || continueChoice == 'N') {
                    running = false;
                } else {
                    // Reset for new order
                    if (currentOrder) {
                        delete currentOrder;
                    }
                    orderBuilder->reset();
                    currentOrder = nullptr;
                }
                break;
            }
            
            case 6: {
                running = false;
                cout << "\nThank you for shopping with us!" << endl;
                break;
            }
            
            default:
                cout << "\n⚠ Invalid choice! Please try again." << endl;
        }
    }
    
    // Cleanup
    cout << "\n=== Cleaning up ===" << endl;
    
    // Clean up order if it exists
    if (currentOrder) {
        delete currentOrder;
    }
    delete orderBuilder;
    delete customer;
    delete dispatcher;
    delete cashier1;
    delete cashier2;
    
    cout << "Goodbye!" << endl;
    
    return 0;
}
