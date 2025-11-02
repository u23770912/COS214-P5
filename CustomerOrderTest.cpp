#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>
#include <climits>
#include <cctype>
#include <cfloat>
#include "Customer.h"
#include "OrderUIFacade.h"
#include "Order.h"
#include "ConcreteOrderBuilder.h"
#include "SinglePlant.h"
#include "PlantBundle.h"
#include "InventoryManager.h"
#include "PlantProduct.h"
#include "SucculentProfile.h"
#include "FlowerProfile.h"
#include "TreeProfile.h"
#include "ReadyForSaleState.h"
#include "Cashier.h"
#include "StaffMember.h"
#include "OrderValidationHandler.h"
#include "PaymentProcessHandler.h"
#include "NotificationHandler.h"
#include "OrderProcessHandler.h"
#include "Command.h"
#include "MoveToSalesFloorCommand.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::getline;
using std::stringstream;

// Input validation utility functions
int getValidInteger(const string& prompt, int min = INT_MIN, int max = INT_MAX) {
    string input;
    int value;
    
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        // Remove leading/trailing whitespace
        input.erase(0, input.find_first_not_of(" \t"));
        input.erase(input.find_last_not_of(" \t") + 1);
        
        if (input.empty()) {
            cout << "Error: Please enter a valid number.\n";
            continue;
        }
        
        stringstream ss(input);
        if (ss >> value && ss.eof()) {
            if (value >= min && value <= max) {
                return value;
            } else {
                cout << "Error: Please enter a number between " << min << " and " << max << ".\n";
            }
        } else {
            cout << "Error: Please enter a valid integer number.\n";
        }
    }
}

double getValidDouble(const string& prompt, double min = -DBL_MAX, double max = DBL_MAX) {
    string input;
    double value;
    
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        // Remove leading/trailing whitespace
        input.erase(0, input.find_first_not_of(" \t"));
        input.erase(input.find_last_not_of(" \t") + 1);
        
        if (input.empty()) {
            cout << "Error: Please enter a valid number.\n";
            continue;
        }
        
        stringstream ss(input);
        if (ss >> value && ss.eof()) {
            if (value >= min && value <= max) {
                return value;
            } else {
                cout << "Error: Please enter a number between " << min << " and " << max << ".\n";
            }
        } else {
            cout << "Error: Please enter a valid decimal number.\n";
        }
    }
}

char getValidChoice(const string& prompt, const string& validChoices) {
    string input;
    
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        // Remove leading/trailing whitespace and convert to lowercase
        input.erase(0, input.find_first_not_of(" \t"));
        input.erase(input.find_last_not_of(" \t") + 1);
        
        if (input.length() == 1) {
            char choice = tolower(input[0]);
            if (validChoices.find(choice) != string::npos) {
                return choice;
            }
        }
        
        cout << "Error: Please enter one of these options: ";
        for (size_t i = 0; i < validChoices.length(); i++) {
            cout << validChoices[i];
            if (i < validChoices.length() - 1) cout << ", ";
        }
        cout << "\n";
    }
}

string getValidString(const string& prompt) {
    string input;
    
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        // Remove leading/trailing whitespace
        input.erase(0, input.find_first_not_of(" \t"));
        input.erase(input.find_last_not_of(" \t") + 1);
        
        if (!input.empty()) {
            return input;
        }
        
        cout << "Error: Please enter a valid non-empty value.\n";
    }
}

// Helper function to populate inventory with sample plants using proper command flow
void populateInventory(SalesFloorStaff* cashierChain) {
    cout << "\n=== Setting up Sales Floor Inventory ===" << endl;
    cout << "Creating plants and processing through staff chain...\n" << endl;
    
    // Create plants in ReadyForSale state
    PlantProduct* aloe1 = new PlantProduct("A001", new SucculentProfile("Aloe Vera", "low", "bright", "sandy"));
    aloe1->transitionTo(new ReadyForSaleState());
    InventoryManager::getInstance().addToGreenhouse(aloe1);
    
    PlantProduct* aloe2 = new PlantProduct("A002", new SucculentProfile("Aloe Vera", "low", "bright", "sandy"));
    aloe2->transitionTo(new ReadyForSaleState());
    InventoryManager::getInstance().addToGreenhouse(aloe2);

    PlantProduct* rose1 = new PlantProduct("R001", new FlowerProfile("Rose", "moderate", "full sun", "loamy"));
    rose1->transitionTo(new ReadyForSaleState());
    InventoryManager::getInstance().addToGreenhouse(rose1);

    PlantProduct* rose2 = new PlantProduct("R002", new FlowerProfile("Rose", "moderate", "full sun", "loamy"));
    rose2->transitionTo(new ReadyForSaleState());
    InventoryManager::getInstance().addToGreenhouse(rose2);

    PlantProduct* oak1 = new PlantProduct("O001", new TreeProfile("Oak", "moderate", "full sun", "well-drained"));
    oak1->transitionTo(new ReadyForSaleState());
    InventoryManager::getInstance().addToGreenhouse(oak1);
    
    // Now properly move plants to sales floor using MoveToSalesFloorCommand through Cashier chain
    std::vector<PlantProduct*> plants;
    plants.push_back(aloe1);
    plants.push_back(aloe2);
    plants.push_back(rose1);
    plants.push_back(rose2);
    plants.push_back(oak1);
    
    cout << "Processing MoveToSalesFloor commands through Cashier chain...\n" << endl;
    
    for (size_t i = 0; i < plants.size(); i++) {
        PlantProduct* plant = plants[i];
        
        // Create MoveToSalesFloorCommand
        Command* moveCommand = Command::createCommand("MoveToSalesFloor");
        if (moveCommand) {
            moveCommand->setReceiver(plant);
            
            cout << "[" << (i+1) << "/" << plants.size() << "] Moving " 
                 << plant->getProfile()->getSpeciesName() 
                 << " (" << plant->getId() << ") to sales floor..." << endl;
            
            // Dispatch through cashier chain (proper Chain of Responsibility)
            cashierChain->handleCommand(moveCommand);
            
            cout << endl;
        } else {
            cout << "ERROR: Failed to create MoveToSalesFloorCommand for " << plant->getId() << endl;
        }
    }

    cout << "Sales floor inventory now has " 
         << InventoryManager::getInstance().getReadyForSalePlants().size() 
         << " plants" << endl;
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
    auto plants = InventoryManager::getInstance().getReadyForSalePlants();

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
    
    // Register command prototypes (needed for Command::createCommand())
    cout << "=== Registering Command Prototypes ===" << endl;
    Command::registerCommand("MoveToSalesFloor", new MoveToSalesFloorCommand());
    cout << "MoveToSalesFloorCommand registered" << endl;
    cout << "======================================\n" << endl;
    
    // Setup cashier staff chain
    cout << "=== Setting up Sales Staff ===" << endl;
    SalesFloorStaff* cashier1 = new Cashier();
    SalesFloorStaff* cashier2 = new Cashier();
    cashier1->setNext(cashier2);
    
    StaffMember* dispatcher = new StaffMember();
    dispatcher->registerTeam("Sales", cashier1);
    cout << "Sales team configured with 2 cashiers" << endl;
    cout << "==================================\n" << endl;
    
    // Setup inventory using proper command flow through cashier chain
    populateInventory(cashier1);
    
    // Create customer
    cout << "=== Customer Information ===" << endl;
    string name = getValidString("Enter your name: ");
    string email = getValidString("Enter your email: ");
    string phone = getValidString("Enter your phone: ");
    
    Customer* customer = new Customer(name, email, phone);
    cout << "\nWelcome, " << name << "!" << endl;
    cout << "===========================\n" << endl;
    
    // Create order builder
    ConcreteOrderBuilder* orderBuilder = new ConcreteOrderBuilder(name);
    Order* currentOrder = nullptr;
    
    bool running = true;
    while (running) {
        displayMenu();
        
        int choice = getValidInteger("Enter your choice: ", 1, 6);
        
        switch (choice) {
            case 1: {
                displayAvailablePlants();
                break;
            }
            
            case 2: {
                // Add single plant
                displayAvailablePlants();
               
                auto plants = InventoryManager::getInstance().getReadyForSalePlants();

                if (plants.empty()) {
                    cout << "\nNo plants available!" << endl;
                    break;
                }
                
                int plantNum = getValidInteger("\nEnter plant number to add (1-" + std::to_string(plants.size()) + "): ", 1, plants.size());
                
                int quantity = getValidInteger("Enter quantity: ", 1, 100);
                
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
                string bundleName = getValidString("Enter bundle name: ");
                
                // We'll calculate automatic discount after adding plants
                PlantBundle* bundle = new PlantBundle(bundleName, "Custom", 1, 0.0);
                
                // Add plants to bundle
                displayAvailablePlants();
                auto plants = InventoryManager::getInstance().getReadyForSalePlants();
                
                if (plants.empty()) {
                    cout << "\nNo plants available for bundle!" << endl;
                    delete bundle;
                    break;
                }
                
                int numPlants = getValidInteger("\nHow many different plants in bundle? ", 1, 10);
                
                int totalPlantCount = 0;
                
                for (int i = 0; i < numPlants; i++) {
                    cout << "\nPlant " << (i+1) << " of " << numPlants << endl;
                    int plantNum = getValidInteger("Enter plant number: ", 1, plants.size());
                    
                    int qty = getValidInteger("Enter quantity: ", 1, 50);
                    totalPlantCount += qty;  // Track total plants for automatic discount
                    
                    string plantType = plants[plantNum-1]->getProfile()->getSpeciesName();
                    SinglePlant* bundlePlant = new SinglePlant(plantType, 25.99, qty);
                    bundle->addItem(bundlePlant);
                }
                
                // Calculate automatic discount based on total plant count
                OrderUIFacade* facade = customer->getUIFacade();
                double automaticDiscount = facade->calculateAutomaticDiscount(totalPlantCount);
                
                // Update bundle with automatic discount
                bundle->setDiscount(automaticDiscount);
                
                cout << "\n[AUTOMATIC DISCOUNT] " << totalPlantCount << " plants = " 
                     << automaticDiscount << "% discount applied!" << endl;
                
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
                char continueChoice = getValidChoice("\nCreate new order? (y/n): ", "yn");
                if (continueChoice == 'n') {
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
    
    // Only delete the head of the chain - the Chain of Responsibility 
    // destructor will automatically clean up all chained handlers
    delete cashier1;  // This will automatically delete cashier2 via ~StaffChainHandler()
    // DO NOT delete cashier2 separately - it's already freed by the chain!
    
    // Clean up inventory BEFORE program exit to avoid static destruction order issues
    InventoryManager::getInstance().cleanup();
    
    cout << "Goodbye!" << endl;
    
    return 0;
}
