#include "TerminalUI.h"
#include "Customer.h"
#include "Order.h"
#include "ConcreteOrderBuilder.h"
#include "OrderDirector.h"
#include "PlantProduct.h"
#include "PlantSpeciesProfile.h"
#include "InventoryManager.h"
#include "SinglePlant.h"
#include "PlantBundle.h"
#include "OrderUIFacade.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>

/**
 * @file TerminalUI.cpp
 * @brief Implementation of the TerminalUI class for all terminal UI operations
 * 
 * This class provides two categories of functionality:
 * 1. System-wide display utilities (used by main.cpp for greenhouse simulation)
 * 2. Customer-facing operations (used by Customer/Order system for sales)
 * 
 * The TerminalUI acts as a central Facade for all terminal display operations,
 * ensuring consistent formatting and color coding throughout the application.
 */

// ============================================================================
// Basic Terminal Control Functions (Used by entire system)
// ============================================================================

void TerminalUI::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        std::cout << "\033[2J\033[1;1H";
    #endif
}

void TerminalUI::setCursorPosition(int row, int col) {
    std::cout << "\033[" << row << ";" << col << "H";
}

void TerminalUI::hideCursor() {
    std::cout << "\033[?25l" << std::flush;
}

void TerminalUI::showCursor() {
    std::cout << "\033[?25h" << std::flush;
}

void TerminalUI::printHeader(const std::string& title, const std::string& subtitle) {
    std::cout << BOLD << CYAN << "\n============================================================\n";
    std::cout << "  " << title << "\n";
    
    if (!subtitle.empty()) {
        std::cout << "  " << subtitle << "\n";
    }
    
    std::cout << "============================================================" << RESET << "\n\n";
}

void TerminalUI::printSection(const std::string& title) {
    std::cout << BOLD << YELLOW << ">> " << title << RESET << std::endl;
}

void TerminalUI::printSubsection(const std::string& title) {
    std::cout << BOLD << BLUE << "  - " << title << RESET << std::endl;
}

void TerminalUI::printSuccess(const std::string& msg) {
    std::cout << GREEN << "[OK] " << msg << RESET << std::endl;
}

void TerminalUI::printError(const std::string& msg) {
    std::cout << RED << "[ERROR] " << msg << RESET << std::endl;
}

void TerminalUI::printWarning(const std::string& msg) {
    std::cout << YELLOW << "[WARN] " << msg << RESET << std::endl;
}

void TerminalUI::printInfo(const std::string& msg) {
    std::cout << BLUE << "[INFO] " << msg << RESET << std::endl;
}

void TerminalUI::printDebug(const std::string& msg) {
    std::cout << GRAY << "[DEBUG] " << msg << RESET << std::endl;
}

void TerminalUI::printBusy(const std::string& name) {
    std::cout << RED << "  [BUSY] " << RESET << name << GRAY << " (working...)" << RESET << std::endl;
}

void TerminalUI::printAvailable(const std::string& name) {
    std::cout << GREEN << "  [FREE] " << RESET << name << std::endl;
}

void TerminalUI::printStaffAssignment(const std::string& plantId,
                                     const std::string& plantState,
                                     const std::string& task) {
    std::cout << GRAY << "    -> " << RESET;
    if (!plantId.empty()) {
        std::cout << CYAN << plantId << RESET;
    } else {
        std::cout << CYAN << "Unknown" << RESET;
    }

    if (!plantState.empty()) {
        std::cout << " [" << BOLD << plantState << RESET << "]";
    }

    if (!task.empty()) {
        std::cout << " | task: " << BOLD << MAGENTA << task << RESET;
    }

    std::cout << std::endl;
}

// ============================================================================
// Customer Order Session Methods (Facade for Customer/Order System)
// ============================================================================

void TerminalUI::displayWelcomeMessage(const std::string& customerName) {
    std::cout << "\n" << BOLD << CYAN << "╔════════════════════════════════════════╗" << RESET << std::endl;
    std::cout << BOLD << CYAN << "║  GREENHOUSE PLANT ORDERING SYSTEM     ║" << RESET << std::endl;
    std::cout << BOLD << CYAN << "╚════════════════════════════════════════╝" << RESET << std::endl;
    std::cout << "\n" << BOLD << "Hello, " << GREEN << customerName << RESET << BOLD << "!" << RESET << std::endl;
    std::cout << "We're excited to help you find the perfect plants for your garden.\n" << std::endl;
}

void TerminalUI::displayAvailablePlants() {
    printSection("AVAILABLE PLANTS FOR SALE");
    
    // Get plants from inventory
    InventoryManager& inventory = InventoryManager::getInstance();
    std::vector<PlantProduct*> plants = inventory.getReadyForSalePlants();
    
    if (plants.empty()) {
        printWarning("No plants currently available in our sales floor.");
        std::cout << "Please check back later or contact staff for assistance.\n" << std::endl;
        return;
    }
    
    std::cout << "\n" << BOLD << "We have " << plants.size() << " plants available:" << RESET << "\n" << std::endl;
    
    // Display plants in a formatted table
    std::cout << BOLD << CYAN;
    std::cout << std::setw(5) << std::left << "No."
              << std::setw(25) << std::left << "Species Name"
              << std::setw(15) << std::left << "Water Needs"
              << std::setw(15) << std::left << "Light Needs"
              << std::setw(15) << std::left << "Soil Type"
              << RESET << std::endl;
    
    std::cout << CYAN;
    for (int i = 0; i < 85; ++i) std::cout << "─";
    std::cout << RESET << std::endl;
    
    for (size_t i = 0; i < plants.size(); ++i) {
        PlantProduct* plant = plants[i];
        PlantSpeciesProfile* profile = plant->getProfile();
        
        std::string waterNeeds = profile->getProperty("idealWater");
        std::string sunNeeds = profile->getProperty("idealSunlight");
        std::string soilType = profile->getProperty("idealSoil");
        
        std::cout << YELLOW << std::setw(5) << std::left << (i + 1) << RESET
                  << std::setw(25) << std::left << profile->getSpeciesName()
                  << std::setw(15) << std::left << (waterNeeds.empty() ? "N/A" : waterNeeds)
                  << std::setw(15) << std::left << (sunNeeds.empty() ? "N/A" : sunNeeds)
                  << std::setw(15) << std::left << (soilType.empty() ? "N/A" : soilType)
                  << std::endl;
    }
    
    std::cout << std::endl;
}

void TerminalUI::displayCurrentOrder(Order* order) {
    if (!order || order->isEmpty()) {
        printSection("CURRENT ORDER");
        printWarning("Your shopping cart is empty.");
        std::cout << "Add some plants to get started!\n" << std::endl;
        return;
    }
    
    printSection("YOUR CURRENT ORDER");
    
    std::cout << "\n" << BOLD << "Order ID: " << RESET << order->getOrderId() << std::endl;
    std::cout << BOLD << "Customer: " << RESET << order->getCustomerName() << std::endl;
    std::cout << BOLD << "Status: " << RESET << order->getStatus() << "\n" << std::endl;
    
    // Display order items
    std::vector<OrderItem*> items = order->getOrderItems();
    
    std::cout << BOLD << "Items in your order:" << RESET << std::endl;
    std::cout << CYAN;
    for (int i = 0; i < 65; ++i) std::cout << "─";
    std::cout << RESET << std::endl;
    
    double subtotal = 0.0;
    for (size_t i = 0; i < items.size(); ++i) {
        OrderItem* item = items[i];
        double itemPrice = item->getPrice();
        subtotal += itemPrice;
        
        std::cout << YELLOW << (i + 1) << ". " << RESET
                  << BOLD << item->getName() << RESET << std::endl;
        std::cout << "   " << item->getDescription() << std::endl;
        std::cout << "   " << GREEN << "Price: $" << std::fixed << std::setprecision(2) 
                  << itemPrice << RESET << std::endl;
        
        // Check if it's a bundle to show discount
        PlantBundle* bundle = dynamic_cast<PlantBundle*>(item);
        if (bundle && bundle->getDiscount() > 0.0) {
            std::cout << "   " << MAGENTA << "Bundle Discount: " << bundle->getDiscount() 
                      << "% off!" << RESET << std::endl;
        }
        
        std::cout << std::endl;
    }
    
    std::cout << CYAN;
    for (int i = 0; i < 65; ++i) std::cout << "─";
    std::cout << RESET << std::endl;
    
    std::cout << BOLD << "Total Items: " << RESET << items.size() << std::endl;
    std::cout << BOLD << GREEN << "Order Total: $" << std::fixed << std::setprecision(2) 
              << subtotal << RESET << "\n" << std::endl;
}

void TerminalUI::displayOrderSummary(Order* order) {
    printHeader("ORDER SUMMARY", "Please review your order before submitting");
    
    if (!order || order->isEmpty()) {
        printWarning("Cannot display summary for empty order.");
        return;
    }
    
    std::cout << order->getOrderSummary() << std::endl;
}

void TerminalUI::displayOrderConfirmation(Order* order, bool success) {
    if (success) {
        printHeader("ORDER CONFIRMED!", "Thank you for your purchase");
        std::cout << "\n" << GREEN << BOLD << "Your order has been successfully processed!" << RESET << "\n" << std::endl;
        
        std::cout << "Order Details:" << std::endl;
        std::cout << "  Order ID: " << order->getOrderId() << std::endl;
        std::cout << "  Status: " << order->getStatus() << std::endl;
        std::cout << "  Total Amount: $" << std::fixed << std::setprecision(2) 
                  << order->getTotalAmount() << std::endl;
        std::cout << "\nA confirmation email will be sent shortly.\n" << std::endl;
    } else {
        printHeader("ORDER PROCESSING FAILED", "We encountered an issue");
        std::cout << "\n" << RED << BOLD << "Unfortunately, we couldn't process your order." << RESET << "\n" << std::endl;
        std::cout << "Possible reasons:" << std::endl;
        std::cout << "  - Some items are out of stock" << std::endl;
        std::cout << "  - Payment processing error" << std::endl;
        std::cout << "  - System error" << std::endl;
        std::cout << "\nPlease review your order and try again, or contact our staff for assistance.\n" << std::endl;
    }
}

void TerminalUI::displayDiscountInformation() {
    printSubsection("Automatic Discount Tiers");
    
    std::cout << "  " << GREEN << "Buy more, save more!" << RESET << std::endl;
    std::cout << "  " << CYAN << "3-5 plants:" << RESET << "   5% discount" << std::endl;
    std::cout << "  " << CYAN << "6-9 plants:" << RESET << "   10% discount" << std::endl;
    std::cout << "  " << CYAN << "10+ plants:" << RESET << "  15% discount" << std::endl;
    std::cout << "\n  Discounts are automatically applied when you add items to your order!\n" << std::endl;
}

void TerminalUI::displayOperationResult(bool success, const std::string& operation, const std::string& details) {
    if (success) {
        printSuccess(operation + " completed successfully");
        if (!details.empty()) {
            std::cout << "  " << GRAY << details << RESET << std::endl;
        }
    } else {
        printError(operation + " failed");
        if (!details.empty()) {
            std::cout << "  " << GRAY << details << RESET << std::endl;
        }
    }
}

void TerminalUI::displayInventoryStatus() {
    printSection("INVENTORY STATUS");
    
    InventoryManager& inventory = InventoryManager::getInstance();
    
    std::cout << "\n" << BOLD << "Sales Floor:" << RESET << std::endl;
    std::cout << "  Plants available: " << GREEN << inventory.getReadyForSalePlants().size() << RESET << std::endl;
    
    std::cout << "\n" << BOLD << "Greenhouse:" << RESET << std::endl;
    std::cout << "  Plants growing: " << YELLOW << inventory.getGreenHouseInventory().size() << RESET << std::endl;
    
    std::cout << "\n" << BOLD << "Total Stock:" << RESET << std::endl;
    std::cout << "  Total plants: " << CYAN << inventory.getStockCount() << RESET << std::endl;
    std::cout << std::endl;
}

// ============================================================================
// Helper Methods for Order Building
// ============================================================================

bool TerminalUI::addPlantToOrder(Order* order, int plantIndex, int quantity) {
    if (!order) {
        printError("Cannot add plant - no active order");
        return false;
    }
    
    InventoryManager& inventory = InventoryManager::getInstance();
    std::vector<PlantProduct*> plants = inventory.getReadyForSalePlants();
    
    // Validate plant index
    if (plantIndex < 0 || plantIndex >= static_cast<int>(plants.size())) {
        printError("Invalid plant selection");
        return false;
    }
    
    // Validate quantity
    if (quantity <= 0) {
        printError("Quantity must be greater than 0");
        return false;
    }
    
    // Get plant information
    PlantProduct* selectedPlant = plants[plantIndex];
    std::string plantType = selectedPlant->getProfile()->getSpeciesName();
    
    // Create SinglePlant order item
    // Note: In a real system, price would come from the plant profile or database
    SinglePlant* plantItem = new SinglePlant(plantType, 25.99, quantity, "Medium");
    
    // Add to order
    order->addOrderItem(plantItem);
    
    printSuccess("Added " + std::to_string(quantity) + "x " + plantType + " to order");
    return true;
}

bool TerminalUI::addBundleToOrder(Order* order, const std::string& bundleName, 
                                  const std::vector<int>& plantIndices, 
                                  const std::vector<int>& quantities, 
                                  double discount) {
    if (!order) {
        printError("Cannot add bundle - no active order");
        return false;
    }
    
    if (plantIndices.size() != quantities.size()) {
        printError("Plant indices and quantities must match");
        return false;
    }
    
    if (plantIndices.empty()) {
        printError("Bundle must contain at least one plant");
        return false;
    }
    
    InventoryManager& inventory = InventoryManager::getInstance();
    std::vector<PlantProduct*> plants = inventory.getReadyForSalePlants();
    
    // Create bundle
    PlantBundle* bundle = new PlantBundle(bundleName, "Custom", 1, discount);
    
    // Add plants to bundle
    for (size_t i = 0; i < plantIndices.size(); ++i) {
        int plantIndex = plantIndices[i];
        int quantity = quantities[i];
        
        // Validate indices
        if (plantIndex < 0 || plantIndex >= static_cast<int>(plants.size())) {
            printError("Invalid plant selection in bundle");
            delete bundle;
            return false;
        }
        
        if (quantity <= 0) {
            printError("Bundle quantity must be greater than 0");
            delete bundle;
            return false;
        }
        
        // Get plant information and add to bundle
        PlantProduct* selectedPlant = plants[plantIndex];
        std::string plantType = selectedPlant->getProfile()->getSpeciesName();
        SinglePlant* bundlePlant = new SinglePlant(plantType, 25.99, quantity, "Medium");
        bundle->addItem(bundlePlant);
    }
    
    // Add bundle to order
    order->addOrderItem(bundle);
    
    std::ostringstream oss;
    oss << "Added bundle '" << bundleName << "' with " << plantIndices.size() 
        << " plant types";
    if (discount > 0.0) {
        oss << " (" << discount << "% discount)";
    }
    printSuccess(oss.str());
    
    return true;
}

void TerminalUI::displayHelp() {
    printSection("HELP & INSTRUCTIONS");
    
    std::cout << "\n" << BOLD << "How to place an order:" << RESET << std::endl;
    std::cout << "  1. Browse available plants using 'View Available Plants'" << std::endl;
    std::cout << "  2. Add individual plants or create bundles" << std::endl;
    std::cout << "  3. Review your order using 'View Current Order'" << std::endl;
    std::cout << "  4. Submit your order for processing" << std::endl;
    
    std::cout << "\n" << BOLD << "Tips:" << RESET << std::endl;
    std::cout << "  - Bundles automatically receive discounts based on quantity" << std::endl;
    std::cout << "  - You can view your order at any time before submitting" << std::endl;
    std::cout << "  - Orders are validated for inventory availability" << std::endl;
    
    std::cout << "\n" << BOLD << "Need assistance?" << RESET << std::endl;
    std::cout << "  Contact our staff at: support@greenhouse.com\n" << std::endl;
}

void TerminalUI::displaySeparator() {
    std::cout << GRAY;
    for (int i = 0; i < 65; ++i) std::cout << "─";
    std::cout << RESET << std::endl;
}

void TerminalUI::displayBlankLine() {
    std::cout << std::endl;
}
