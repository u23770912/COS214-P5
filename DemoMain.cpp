#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <thread>
#include <chrono>
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
#include "SuggestionTemplate/BouquetSuggestionFactory.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

// ANSI Color Codes
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BG_GREEN "\033[42m"
#define BG_BLUE  "\033[44m"

// UI Helper Functions
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void printBorder(char ch = '═', int width = 70) {
    cout << string(width, ch) << endl;
}

void printCentered(const string& text, int width = 70) {
    int padding = (width - text.length()) / 2;
    cout << string(padding, ' ') << text << endl;
}

void animateText(const string& text, int delayMs = 30) {
    for (char c : text) {
        cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
    cout << endl;
}

void showLoadingBar(const string& task, int durationMs = 1000) {
    cout << CYAN << task << ": " << RESET;
    int bars = 30;
    for (int i = 0; i <= bars; i++) {
        cout << GREEN << "█" << RESET << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(durationMs / bars));
    }
    cout << " " << GREEN << "✓" << RESET << endl;
}

void printHeader() {
    clearScreen();
    cout << GREEN << BOLD;
    cout << "\n";
    cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
    cout << "    ║                                                                  ║\n";
    cout << "    ║          🌿 GREENHOUSE NURSERY MANAGEMENT SYSTEM 🌿             ║\n";
    cout << "    ║                                                                  ║\n";
    cout << "    ║              Your Gateway to Green Living                       ║\n";
    cout << "    ║                                                                  ║\n";
    cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
    cout << RESET << "\n";
}

void populateInventory() {
    InventoryManager& inventory = InventoryManager::getInstance();
    
    cout << CYAN << "\n⚙  Initializing Greenhouse Inventory..." << RESET << endl;
    showLoadingBar("Loading plant database", 800);
    
    // Succulents
    PlantProduct* aloe1 = new PlantProduct("A001", new SucculentProfile("Aloe Vera", "low", "bright", "sandy"));
    aloe1->transitionTo(new ReadyForSaleState());
    inventory.moveToSalesFloor(aloe1);
    
    PlantProduct* aloe2 = new PlantProduct("A002", new SucculentProfile("Aloe Vera", "low", "bright", "sandy"));
    aloe2->transitionTo(new ReadyForSaleState());
    inventory.moveToSalesFloor(aloe2);
    
    PlantProduct* cactus1 = new PlantProduct("C001", new SucculentProfile("Desert Cactus", "minimal", "full sun", "rocky"));
    cactus1->transitionTo(new ReadyForSaleState());
    inventory.moveToSalesFloor(cactus1);
    
    // Flowers
    PlantProduct* rose1 = new PlantProduct("R001", new FlowerProfile("Rose", "moderate", "full sun", "loamy"));
    rose1->transitionTo(new ReadyForSaleState());
    inventory.moveToSalesFloor(rose1);
    
    PlantProduct* rose2 = new PlantProduct("R002", new FlowerProfile("Rose", "moderate", "full sun", "loamy"));
    rose2->transitionTo(new ReadyForSaleState());
    inventory.moveToSalesFloor(rose2);
    
    PlantProduct* tulip1 = new PlantProduct("T001", new FlowerProfile("Tulip", "moderate", "partial sun", "fertile"));
    tulip1->transitionTo(new ReadyForSaleState());
    inventory.moveToSalesFloor(tulip1);
    
    PlantProduct* lily1 = new PlantProduct("L001", new FlowerProfile("Lily", "moderate", "partial shade", "rich"));
    lily1->transitionTo(new ReadyForSaleState());
    inventory.moveToSalesFloor(lily1);
    
    // Trees
    PlantProduct* oak1 = new PlantProduct("O001", new TreeProfile("Oak", "moderate", "full sun", "well-drained"));
    oak1->transitionTo(new ReadyForSaleState());
    inventory.moveToSalesFloor(oak1);
    
    PlantProduct* maple1 = new PlantProduct("M001", new TreeProfile("Maple", "moderate", "partial sun", "moist"));
    maple1->transitionTo(new ReadyForSaleState());
    inventory.moveToSalesFloor(maple1);
    
    cout << GREEN << "✓ Inventory ready: " << inventory.getReadyForSalePlants().size() 
         << " plants available\n" << RESET << endl;
}

void displayMainMenu() {
    cout << "\n" << CYAN << BOLD;
    cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
    cout << "    ║                        MAIN MENU                                 ║\n";
    cout << "    ╠══════════════════════════════════════════════════════════════════╣\n";
    cout << RESET;
    
    cout << "    ║  " << GREEN << "1" << RESET << " │ 🌺 Browse Available Plants                                  ║\n";
    cout << "    ║  " << GREEN << "2" << RESET << " │ 🛒 Add Single Plant to Cart                                 ║\n";
    cout << "    ║  " << GREEN << "3" << RESET << " │ 🎁 Create Custom Plant Bundle                               ║\n";
    cout << "    ║  " << GREEN << "4" << RESET << " │ 💐 Browse Event Bouquet Suggestions                         ║\n";
    cout << "    ║  " << GREEN << "5" << RESET << " │ 👀 View Current Order                                       ║\n";
    cout << "    ║  " << GREEN << "6" << RESET << " │ 💾 Save Order Snapshot (Memento)                            ║\n";
    cout << "    ║  " << GREEN << "7" << RESET << " │ ⏮  Restore Last Order Snapshot                              ║\n";
    cout << "    ║  " << GREEN << "8" << RESET << " │ 💳 Checkout & Payment                                       ║\n";
    cout << "    ║  " << GREEN << "9" << RESET << " │ 🚪 Exit                                                     ║\n";
    
    cout << CYAN << BOLD;
    cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
    cout << RESET;
    cout << "\n    " << YELLOW << "➤ Enter your choice: " << RESET;
}

void displayPlantCatalog() {
    InventoryManager& inventory = InventoryManager::getInstance();
    auto plants = inventory.getReadyForSalePlants();
    
    clearScreen();
    cout << "\n" << GREEN << BOLD;
    cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
    cout << "    ║                    🌿 PLANT CATALOG 🌿                           ║\n";
    cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
    cout << RESET << "\n";
    
    if (plants.empty()) {
        cout << RED << "    ✗ No plants available at the moment.\n" << RESET;
        return;
    }
    
    // Group plants by category
    cout << CYAN << "    Available Plants: " << plants.size() << "\n" << RESET;
    cout << "    " << string(66, '─') << "\n";
    
    for (size_t i = 0; i < plants.size(); i++) {
        string name = plants[i]->getProfile()->getSpeciesName();
        string water = plants[i]->getProfile()->getProperty("idealWater");
        string sun = plants[i]->getProfile()->getProperty("idealSunlight");
        
        cout << "    " << YELLOW << std::setw(2) << (i+1) << ". " << RESET;
        cout << BOLD << std::setw(20) << std::left << name << RESET;
        cout << " │ 💧 " << std::setw(12) << water;
        cout << " │ ☀️  " << std::setw(15) << sun;
        cout << " │ " << GREEN << "$25.99" << RESET << "\n";
    }
    
    cout << "    " << string(66, '─') << "\n";
}

void addSinglePlantToOrder(Order*& currentOrder, ConcreteOrderBuilder* builder) {
    displayPlantCatalog();
    
    InventoryManager& inventory = InventoryManager::getInstance();
    auto plants = inventory.getReadyForSalePlants();
    
    if (plants.empty()) {
        cout << "\n    " << RED << "Press Enter to continue..." << RESET;
        cin.ignore();
        cin.get();
        return;
    }
    
    cout << "\n    " << YELLOW << "➤ Enter plant number (1-" << plants.size() << "): " << RESET;
    int plantNum;
    cin >> plantNum;
    
    if (plantNum < 1 || plantNum > (int)plants.size()) {
        cout << RED << "    ✗ Invalid selection!\n" << RESET;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return;
    }
    
    cout << "    " << YELLOW << "➤ Quantity: " << RESET;
    int quantity;
    cin >> quantity;
    
    if (quantity < 1) {
        cout << RED << "    ✗ Invalid quantity!\n" << RESET;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return;
    }
    
    // Create order if doesn't exist
    if (!currentOrder) {
        currentOrder = builder->getOrder();
    }
    
    string plantType = plants[plantNum-1]->getProfile()->getSpeciesName();
    SinglePlant* plant = new SinglePlant(plantType, 25.99, quantity);
    currentOrder->addOrderItem(plant);
    
    cout << "\n    " << GREEN << "✓ Added " << quantity << "x " << plantType << " to cart!\n" << RESET;
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
}

void createPlantBundle(Order*& currentOrder, ConcreteOrderBuilder* builder) {
    clearScreen();
    cout << "\n" << MAGENTA << BOLD;
    cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
    cout << "    ║                  🎁 CREATE CUSTOM BUNDLE 🎁                      ║\n";
    cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
    cout << RESET << "\n";
    
    cin.ignore();
    cout << "    " << YELLOW << "➤ Bundle name: " << RESET;
    string bundleName;
    std::getline(cin, bundleName);
    
    cout << "    " << YELLOW << "➤ Discount % (0-50): " << RESET;
    double discount;
    cin >> discount;
    
    if (discount < 0 || discount > 50) {
        cout << RED << "    ✗ Invalid discount!\n" << RESET;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return;
    }
    
    PlantBundle* bundle = new PlantBundle(bundleName, "Custom", 1, discount);
    
    displayPlantCatalog();
    
    InventoryManager& inventory = InventoryManager::getInstance();
    auto plants = inventory.getReadyForSalePlants();
    
    if (plants.empty()) {
        delete bundle;
        cout << "\n    " << RED << "Press Enter to continue..." << RESET;
        cin.get();
        return;
    }
    
    cout << "\n    " << YELLOW << "➤ Number of plant types in bundle: " << RESET;
    int numTypes;
    cin >> numTypes;
    
    for (int i = 0; i < numTypes; i++) {
        cout << "\n    " << CYAN << "Plant " << (i+1) << " of " << numTypes << RESET << "\n";
        cout << "    " << YELLOW << "➤ Plant number: " << RESET;
        int plantNum;
        cin >> plantNum;
        
        if (plantNum < 1 || plantNum > (int)plants.size()) {
            cout << RED << "    ✗ Skipping invalid selection...\n" << RESET;
            continue;
        }
        
        cout << "    " << YELLOW << "➤ Quantity: " << RESET;
        int qty;
        cin >> qty;
        
        string plantType = plants[plantNum-1]->getProfile()->getSpeciesName();
        SinglePlant* bundlePlant = new SinglePlant(plantType, 25.99, qty);
        bundle->addItem(bundlePlant);
        
        cout << GREEN << "    ✓ Added " << qty << "x " << plantType << "\n" << RESET;
    }
    
    if (!currentOrder) {
        currentOrder = builder->getOrder();
    }
    
    currentOrder->addOrderItem(bundle);
    cout << "\n    " << GREEN << BOLD << "✓ Bundle '" << bundleName << "' created successfully!\n" << RESET;
    std::this_thread::sleep_for(std::chrono::milliseconds(1200));
}

void browseBouquetSuggestions() {
    clearScreen();
    cout << "\n" << MAGENTA << BOLD;
    cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
    cout << "    ║              💐 EVENT BOUQUET SUGGESTIONS 💐                     ║\n";
    cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
    cout << RESET << "\n";
    
    BouquetSuggestionFactory& factory = BouquetSuggestionFactory::getInstance();
    auto events = factory.getAvailableEvents();
    
    cout << "    Available Events:\n";
    cout << "    " << string(66, '─') << "\n";
    for (size_t i = 0; i < events.size(); i++) {
        cout << "    " << YELLOW << (i+1) << ". " << RESET << events[i] << "\n";
    }
    cout << "    " << string(66, '─') << "\n";
    
    cout << "\n    " << YELLOW << "➤ Select event (1-" << events.size() << "): " << RESET;
    int choice;
    cin >> choice;
    
    if (choice < 1 || choice > (int)events.size()) {
        cout << RED << "    ✗ Invalid selection!\n" << RESET;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return;
    }
    
    string eventType = events[choice-1];
    auto tmpl = factory.getTemplate(eventType);
    
    if (!tmpl) {
        cout << RED << "    ✗ Template not found!\n" << RESET;
        return;
    }
    
    clearScreen();
    cout << "\n";
    auto suggestions = tmpl->generateSuggestions();
    
    cout << "\n    " << CYAN << "Press Enter to return to menu..." << RESET;
    cin.ignore();
    cin.get();
}

void viewCurrentOrder(Order* currentOrder) {
    clearScreen();
    if (!currentOrder || currentOrder->isEmpty()) {
        cout << "\n" << YELLOW;
        cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
        cout << "    ║                     🛒 YOUR CART IS EMPTY                        ║\n";
        cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
        cout << RESET << "\n";
        cout << "    Start shopping by adding plants to your cart!\n";
    } else {
        cout << "\n" << CYAN << BOLD;
        cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
        cout << "    ║                      🛒 YOUR CURRENT ORDER                       ║\n";
        cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
        cout << RESET << "\n";
        
        string summary = currentOrder->getOrderSummary();
        // Indent the summary
        size_t pos = 0;
        while ((pos = summary.find('\n', pos)) != string::npos) {
            summary.insert(pos + 1, "    ");
            pos += 5;
        }
        cout << "    " << summary << "\n";
    }
    
    cout << "\n    " << CYAN << "Press Enter to continue..." << RESET;
    cin.ignore();
    cin.get();
}

void checkoutOrder(Order* currentOrder, Customer* customer) {
    if (!currentOrder || currentOrder->isEmpty()) {
        clearScreen();
        cout << "\n" << RED;
        cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
        cout << "    ║                    ✗ CART IS EMPTY                               ║\n";
        cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
        cout << RESET << "\n";
        cout << "    Add items before checking out!\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return;
    }
    
    clearScreen();
    cout << "\n" << GREEN << BOLD;
    cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
    cout << "    ║                    💳 CHECKOUT PROCESS                           ║\n";
    cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
    cout << RESET << "\n";
    
    // Show order summary
    cout << "    " << CYAN << "Order Summary:\n" << RESET;
    cout << "    Items: " << currentOrder->getItemCount() << "\n";
    cout << "    Total: " << GREEN << "$" << std::fixed << std::setprecision(2) 
         << currentOrder->getTotalAmount() << RESET << "\n\n";
    
    // Payment method selection
    cout << "    " << YELLOW << "Select Payment Method:\n" << RESET;
    cout << "    " << string(66, '─') << "\n";
    cout << "    1. 💵 Cash\n";
    cout << "    2. 💳 Credit Card\n";
    cout << "    3. 🏦 EFT (Bank Transfer)\n";
    cout << "    " << string(66, '─') << "\n";
    cout << "\n    " << YELLOW << "➤ Choice (1-3): " << RESET;
    
    int paymentChoice;
    cin >> paymentChoice;
    
    string paymentType, paymentDetails;
    
    switch(paymentChoice) {
        case 1:
            paymentType = "CASH";
            paymentDetails = "CASH";
            break;
        case 2: {
            paymentType = "CREDIT_CARD";
            cout << "\n    " << YELLOW << "➤ Card Number: " << RESET;
            string card;
            cin >> card;
            cout << "    " << YELLOW << "➤ Expiry (MM/YY): " << RESET;
            string expiry;
            cin >> expiry;
            cout << "    " << YELLOW << "➤ CVV: " << RESET;
            string cvv;
            cin >> cvv;
            paymentDetails = card + ";" + expiry + ";" + cvv;
            break;
        }
        case 3:
            paymentType = "EFT";
            paymentDetails = "EFT";
            break;
        default:
            cout << RED << "    ✗ Invalid payment method!\n" << RESET;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            return;
    }
    
    cout << "\n";
    showLoadingBar("Processing order", 1500);
    showLoadingBar("Validating inventory", 1000);
    showLoadingBar("Processing payment", 1200);
    
    // Use the integrated payment system
    bool success = customer->executeOrderWithPayment(paymentType, paymentDetails);
    
    if (success) {
        cout << "\n" << GREEN << BOLD;
        cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
        cout << "    ║                  ✓ ORDER SUCCESSFUL!                             ║\n";
        cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
        cout << RESET << "\n";
        cout << "    Thank you for your purchase!\n";
        cout << "    Order ID: " << currentOrder->getOrderId() << "\n";
        cout << "    A confirmation has been sent to your email.\n";
    } else {
        cout << "\n" << RED << BOLD;
        cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
        cout << "    ║                  ✗ ORDER FAILED                                  ║\n";
        cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
        cout << RESET << "\n";
        cout << "    Please check the details and try again.\n";
    }
    
    cout << "\n    " << CYAN << "Press Enter to continue..." << RESET;
    cin.ignore();
    cin.get();
}

int main() {
    printHeader();
    
    cout << CYAN;
    animateText("    Initializing System...", 20);
    cout << RESET;
    
    populateInventory();
    
    // Setup staff
    SalesFloorStaff* cashier1 = new Cashier();
    SalesFloorStaff* cashier2 = new Cashier();
    cashier1->setNext(cashier2);
    
    StaffMember* dispatcher = new StaffMember();
    dispatcher->registerTeam("Sales", cashier1);
    
    // Customer registration
    clearScreen();
    printHeader();
    
    cout << YELLOW << BOLD;
    cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
    cout << "    ║                    CUSTOMER REGISTRATION                         ║\n";
    cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
    cout << RESET << "\n";
    
    string name, email, phone;
    cout << "    " << CYAN << "➤ Full Name: " << RESET;
    std::getline(cin, name);
    cout << "    " << CYAN << "➤ Email: " << RESET;
    std::getline(cin, email);
    cout << "    " << CYAN << "➤ Phone: " << RESET;
    std::getline(cin, phone);
    
    Customer* customer = new Customer(name, email, phone);
    ConcreteOrderBuilder* orderBuilder = new ConcreteOrderBuilder(name);
    Order* currentOrder = nullptr;
    
    cout << "\n    " << GREEN << "✓ Welcome, " << name << "! 🌿\n" << RESET;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    bool running = true;
    while (running) {
        clearScreen();
        printHeader();
        displayMainMenu();
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1:
                displayPlantCatalog();
                cout << "\n    " << CYAN << "Press Enter to continue..." << RESET;
                cin.ignore();
                cin.get();
                break;
                
            case 2:
                addSinglePlantToOrder(currentOrder, orderBuilder);
                break;
                
            case 3:
                createPlantBundle(currentOrder, orderBuilder);
                break;
                
            case 4:
                browseBouquetSuggestions();
                break;
                
            case 5:
                viewCurrentOrder(currentOrder);
                break;
                
            case 6: {
                // Save order snapshot (Memento)
                if (!currentOrder || currentOrder->isEmpty()) {
                    cout << "\n    " << YELLOW << "⚠ No order to save!\n" << RESET;
                } else {
                    customer->saveCurrentOrder();
                    cout << "\n    " << GREEN << "✓ Order snapshot saved!\n" << RESET;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                break;
            }
                
            case 7: {
                // Restore order snapshot (Memento)
                if (!currentOrder) {
                    currentOrder = orderBuilder->getOrder();
                }
                customer->restoreLastOrder();
                cout << "\n    " << GREEN << "✓ Order restored from snapshot!\n" << RESET;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                break;
            }
                
            case 8:
                checkoutOrder(currentOrder, customer);
                // Reset order after checkout
                if (currentOrder) {
                    delete currentOrder;
                    currentOrder = nullptr;
                }
                orderBuilder->reset();
                break;
                
            case 9:
                running = false;
                clearScreen();
                printHeader();
                cout << "\n" << GREEN;
                animateText("    Thank you for visiting the Greenhouse! 🌿", 30);
                cout << "    Have a wonderful day!\n" << RESET << "\n";
                break;
                
            default:
                cout << "\n    " << RED << "✗ Invalid choice! Try again.\n" << RESET;
                std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    
    // Cleanup
    if (currentOrder) delete currentOrder;
    delete orderBuilder;
    delete customer;
    delete dispatcher;
    delete cashier1;
    delete cashier2;
    
    return 0;
}