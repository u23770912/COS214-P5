#include "OrderUIFacade.h"
#include "Customer.h"
#include "Order.h"
#include "PlantProduct.h"
#include "ConcreteOrderBuilder.h"
#include "InventoryManager.h"
#include "PlantSpeciesProfile.h"
#include "TerminalUI.h"
#include "SinglePlant.h"
#include "Pot.h"
#include "PotFactory.h"
#include "ClayPotFactory.h"
#include "PlasticPotFactory.h"
#include "MetalPotFactory.h"
#include "GlassPotFactory.h"
#include "WoodenPotFactory.h"
#include "PotDecorator/PotDecorator.h"
#include "PotDecorator/ColorDecorator.h"
#include "PotDecorator/FinishDecorator.h"
#include "PotDecorator/PatternDecorator.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>

/**
 * @file OrderUIFacade.cpp
 * @brief Implementation of OrderUIFacade - works in conjunction with TerminalUI
 * 
 * OrderUIFacade handles order-specific logic and delegates display operations to TerminalUI.
 * This creates a layered facade:
 * - TerminalUI: Low-level display operations
 * - OrderUIFacade: Order business logic + UI coordination + Pot customization
 * - Customer: High-level order management
 */

// ANSI Color Codes
#define ANSI_RESET   "\033[0m"
#define ANSI_BOLD    "\033[1m"
#define ANSI_RED     "\033[31m"
#define ANSI_GREEN   "\033[32m"
#define ANSI_YELLOW  "\033[33m"
#define ANSI_BLUE    "\033[34m"
#define ANSI_MAGENTA "\033[35m"
#define ANSI_CYAN    "\033[36m"

OrderUIFacade::OrderUIFacade(Customer* customer) : customer(customer) {
    // Facade initialized with customer reference
}

OrderUIFacade::~OrderUIFacade() {
    // Note: We don't delete the customer as it's managed externally
}

void OrderUIFacade::setCustomer(Customer* customer) {
    this->customer = customer;
}

Customer* OrderUIFacade::getCustomer() const {
    return customer;
}

double OrderUIFacade::calculateAutomaticDiscount(int totalPlants) const {
    // Automatic discount logic based on order size
    if (totalPlants >= 10) {
        return 30.0; // Maximum discount for 10+ plants
    } else if (totalPlants >= 6) {
        return 15.0; // Mid-tier discount for 6-9 plants
    } else if (totalPlants >= 3) {
        return 10.0; // Basic discount for 3-5 plants
    }
    return 0.0; // No discount for orders under 3 plants
}

int OrderUIFacade::countTotalPlantsInOrder(ConcreteOrderBuilder* builder) const {
    if (!builder || !builder->hasCurrentOrder()) {
        return 0;
    }
    
    // Get temporary order to count plants
    Order* tempOrder = builder->getOrder();
    if (!tempOrder) {
        return 0;
    }
    
    int totalPlants = 0;
    std::vector<class OrderItem*> items = tempOrder->getOrderItems();
    
    // Count total plants across all order items
    // Note: This is a simplified count - in a real system you'd have more complex logic
    totalPlants = static_cast<int>(items.size());
    
    return totalPlants;
}

void OrderUIFacade::displayAvailableItems() {
    // Delegate to TerminalUI for consistent display formatting
    // TerminalUI handles all the visual presentation
    TerminalUI::displayAvailablePlants();
}

void OrderUIFacade::viewCurrentOrder() {
    if (!customer) {
        TerminalUI::printError("No customer associated with UI facade.");
        return;
    }
    
    ConcreteOrderBuilder* builder = customer->getOrderBuilder();
    if (!builder || !builder->hasCurrentOrder()) {
        // Delegate to TerminalUI for empty order display
        TerminalUI::displayCurrentOrder(0);
        displayDiscountInformation();
        return;
    }
    
    // Get temporary order to display
    Order* tempOrder = builder->getOrder();
    
    // Delegate to TerminalUI for order display
    TerminalUI::displayCurrentOrder(tempOrder);
    
    // Display discount information using TerminalUI methods
    if (tempOrder && !tempOrder->isEmpty()) {
        int plantCount = countTotalPlantsInOrder(builder);
        double discount = calculateAutomaticDiscount(plantCount);
        
        std::string discountInfo = "Total plants: " + std::to_string(plantCount);
        if (discount > 0.0) {
            discountInfo += " | Automatic discount: " + std::to_string(static_cast<int>(discount)) + "%";
            TerminalUI::printSuccess(discountInfo);
        } else {
            discountInfo += " | No discount yet (add 3+ for discount)";
            TerminalUI::printInfo(discountInfo);
        }
    }
}

bool OrderUIFacade::addPlantToOrderWithAutoDiscount(int plantIndex, int quantity) {
    if (!customer) {
        std::cout << "[ERROR] No customer associated with UI facade." << std::endl;
        return false;
    }
    
    std::vector<PlantProduct*> availablePlants = getAvailablePlantsFromInventory();
    
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
    
    // Notify customer observers about the interaction
    customer->notifyInteraction("PlantSelection", 
        "Customer selected " + std::to_string(quantity) + "x " + plantType);
    
    // Use customer's builder to add the plant
    ConcreteOrderBuilder* builder = customer->getOrderBuilder();
    if (builder) {
        builder->buildPlant(plantType, quantity);
        
        // Calculate and show potential discount after addition
        int newPlantCount = countTotalPlantsInOrder(builder);
        double discount = calculateAutomaticDiscount(newPlantCount);
        
        std::cout << "[COMPLETE] Plant added successfully!" << std::endl;
        std::cout << "[DISCOUNT] Total plants now: " << newPlantCount;
        if (discount > 0.0) {
            std::cout << " | Automatic discount: " << discount << "%" << std::endl;
        } else {
            std::cout << " | Add more plants for discounts!" << std::endl;
        }
        
        return true;
    }
    
    std::cout << "[ERROR] Unable to access order builder." << std::endl;
    return false;
}

bool OrderUIFacade::addBundleToOrderWithAutoDiscount(const std::string& bundleName, 
                                                     const std::vector<int>& plantIndices) {
    if (!customer) {
        std::cout << "[ERROR] No customer associated with UI facade." << std::endl;
        return false;
    }
    
    std::vector<PlantProduct*> availablePlants = getAvailablePlantsFromInventory();
    
    if (plantIndices.empty()) {
        std::cout << "[ERROR] Bundle must contain at least one plant." << std::endl;
        return false;
    }
    
    // Validate all indices
    for (size_t i = 0; i < plantIndices.size(); ++i) {
        int index = plantIndices[i];
        if (index < 1 || index > static_cast<int>(availablePlants.size())) {
            std::cout << "[ERROR] Invalid plant number " << index << ". Please choose between 1 and " 
                      << availablePlants.size() << std::endl;
            return false;
        }
    }
    
    // Calculate automatic discount based on bundle size
    double autoDiscount = calculateAutomaticDiscount(static_cast<int>(plantIndices.size()));
    
    std::cout << "\n[BUNDLE] Creating bundle: " << bundleName << " with " 
              << static_cast<int>(plantIndices.size()) << " plants" << std::endl;
    std::cout << "[AUTO-DISCOUNT] Applying " << autoDiscount << "% discount" << std::endl;
    
    // Notify customer observers about bundle creation
    customer->notifyInteraction("BundleCreation", 
        "Customer creating " + bundleName + " with " + std::to_string(plantIndices.size()) + " plants");
    
    // Use customer's builder to create bundle with automatic discount
    ConcreteOrderBuilder* builder = customer->getOrderBuilder();
    if (builder) {
        builder->buildCustomBundle(bundleName, "Mixed", autoDiscount);
        
        // Add each plant to the bundle
        for (size_t i = 0; i < plantIndices.size(); ++i) {
            int index = plantIndices[i];
            PlantProduct* plant = availablePlants[index - 1];
            std::string plantType = plant->getProfile()->getSpeciesName();
            builder->addPlantToCurrentBundle(plantType, 1);
            std::cout << "  [OK] Added " << plantType << " to bundle" << std::endl;
        }
        
        std::cout << "[BUNDLE] Bundle created successfully with automatic discount!" << std::endl;
        return true;
    }
    
    std::cout << "[ERROR] Unable to access order builder." << std::endl;
    return false;
}

void OrderUIFacade::displayOrderCreationWelcome() {
    if (!customer) {
        TerminalUI::printError("No customer associated with UI facade.");
        return;
    }
    
    // Delegate to TerminalUI for welcome message
    TerminalUI::displayWelcomeMessage(customer->getName());
    displayOrderInstructions();
}

void OrderUIFacade::displayOrderExecutionSummary() {
    if (!customer) {
        TerminalUI::printError("No customer associated with UI facade.");
        return;
    }
    
    TerminalUI::printSection("EXECUTING ORDER");
    TerminalUI::printInfo("Customer: " + customer->getName() + " (" + customer->getEmail() + ")");
}

void OrderUIFacade::displayFinalOrderConfirmation() {
    TerminalUI::printSection("ORDER FINALIZED");
    TerminalUI::printSuccess("Order is ready for execution!");
    TerminalUI::printInfo("Use executeOrder() to place your order with staff.");
}

void OrderUIFacade::displayDiscountInformation() const {
    // Delegate to TerminalUI for consistent discount display
    TerminalUI::displayDiscountInformation();
}

void OrderUIFacade::displayOrderInstructions() const {
    TerminalUI::printSubsection("How to build your order:");
    std::cout << "  1. displayAvailableItems() - See what's available" << std::endl;
    std::cout << "  2. addPlantToOrder(index, quantity) - Add plants with auto-discount" << std::endl;
    std::cout << "  3. addBundleToOrder(name, indices) - Create bundles with auto-discount" << std::endl;
    std::cout << "  4. viewCurrentOrder() - Review your order" << std::endl;
    std::cout << "  5. finalizeOrder() - Complete the order" << std::endl;
    TerminalUI::displayBlankLine();
}

// Private helper methods moved from Customer class

std::vector<PlantProduct*> OrderUIFacade::getAvailablePlantsFromInventory() {
    InventoryManager& inventory = InventoryManager::getInstance();
    return inventory.getReadyForSalePlants();
}

void OrderUIFacade::displayPlantDetails(const PlantProduct* plant, int index) {
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

// ============================================================================
// POT ORDERING FUNCTIONALITY - Integrated into Facade
// ============================================================================

void OrderUIFacade::displayPotCatalog() {
    std::cout << "\n" << ANSI_CYAN << ANSI_BOLD;
    std::cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
    std::cout << "    ║                      🏺 POT CATALOG 🏺                           ║\n";
    std::cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
    std::cout << ANSI_RESET << "\n";
    
    std::cout << "    " << ANSI_YELLOW << "1. " << ANSI_RESET << ANSI_BOLD << "Clay Pot" << ANSI_RESET 
              << "     - Traditional, breathable material    " << ANSI_GREEN << "(R10.00)" << ANSI_RESET << "\n";
    std::cout << "    " << ANSI_YELLOW << "2. " << ANSI_RESET << ANSI_BOLD << "Plastic Pot" << ANSI_RESET 
              << "  - Lightweight, durable                " << ANSI_GREEN << "(R10.00)" << ANSI_RESET << "\n";
    std::cout << "    " << ANSI_YELLOW << "3. " << ANSI_RESET << ANSI_BOLD << "Metal Pot" << ANSI_RESET 
              << "    - Modern, industrial look             " << ANSI_GREEN << "(R10.00)" << ANSI_RESET << "\n";
    std::cout << "    " << ANSI_YELLOW << "4. " << ANSI_RESET << ANSI_BOLD << "Glass Pot" << ANSI_RESET 
              << "    - Elegant, transparent design         " << ANSI_GREEN << "(R10.00)" << ANSI_RESET << "\n";
    std::cout << "    " << ANSI_YELLOW << "5. " << ANSI_RESET << ANSI_BOLD << "Wooden Pot" << ANSI_RESET 
              << "   - Rustic, natural aesthetic           " << ANSI_GREEN << "(R10.00)" << ANSI_RESET << "\n\n";
}

void OrderUIFacade::displayPotCustomizationOptions() {
    std::cout << "\n" << ANSI_MAGENTA << ANSI_BOLD;
    std::cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
    std::cout << "    ║                   🎨 CUSTOMIZATION OPTIONS 🎨                    ║\n";
    std::cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
    std::cout << ANSI_RESET << "\n";
    
    std::cout << "    " << ANSI_YELLOW << "1. " << ANSI_RESET << "Add Color       " 
              << ANSI_CYAN << "(Standard: R3.00, Metallic: R5.00)" << ANSI_RESET << "\n";
    std::cout << "    " << ANSI_YELLOW << "2. " << ANSI_RESET << "Add Finish      " 
              << ANSI_CYAN << "(R2.00 - R4.00)" << ANSI_RESET << "\n";
    std::cout << "    " << ANSI_YELLOW << "3. " << ANSI_RESET << "Add Pattern     " 
              << ANSI_CYAN << "(R4.00 - R8.00)" << ANSI_RESET << "\n";
    std::cout << "    " << ANSI_YELLOW << "4. " << ANSI_RESET << "Finish and Add to Cart\n\n";
}

void OrderUIFacade::displayPotConfigSummary(const PotCustomization& config) {
    std::cout << "\n" << ANSI_CYAN << ANSI_BOLD;
    std::cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
    std::cout << "    ║                     POT CONFIGURATION                            ║\n";
    std::cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
    std::cout << ANSI_RESET << "\n";
    
    std::cout << "    Type:     " << ANSI_BOLD << config.potType << ANSI_RESET << "\n";
    std::cout << "    Size:     " << config.size << "\n";
    std::cout << "    Shape:    " << config.shape << "\n";
    std::cout << "    Drainage: " << (config.hasDrainage ? "Yes" : "No") << "\n";
    
    if (!config.color.empty()) {
        std::cout << "    Color:    " << ANSI_YELLOW << config.color << ANSI_RESET << "\n";
    }
    if (!config.finish.empty()) {
        std::cout << "    Finish:   " << ANSI_MAGENTA << config.finish << ANSI_RESET << "\n";
    }
    if (!config.pattern.empty()) {
        std::cout << "    Pattern:  " << ANSI_BLUE << config.pattern << ANSI_RESET << "\n";
    }
    
    std::cout << "\n    " << ANSI_GREEN << ANSI_BOLD << "Total Price: R" 
              << std::fixed << std::setprecision(2) << config.totalPrice << ANSI_RESET << "\n\n";
}

std::string OrderUIFacade::getPotDescription(const PotCustomization& config) {
    std::ostringstream desc;
    desc << config.size << " " << config.shape << " " << config.potType << " Pot";
    
    if (!config.color.empty()) {
        desc << " (" << config.color;
        if (!config.finish.empty()) {
            desc << ", " << config.finish;
        }
        if (!config.pattern.empty()) {
            desc << ", " << config.pattern;
        }
        desc << ")";
    } else if (!config.finish.empty()) {
        desc << " (" << config.finish;
        if (!config.pattern.empty()) {
            desc << ", " << config.pattern;
        }
        desc << ")";
    } else if (!config.pattern.empty()) {
        desc << " (" << config.pattern << ")";
    }
    
    return desc.str();
}

OrderUIFacade::PotCustomization OrderUIFacade::interactivePotConfiguration() {
    PotCustomization config;
    
    // Select pot type
    displayPotCatalog();
    std::cout << "    " << ANSI_YELLOW << "➤ Select pot type (1-5): " << ANSI_RESET;
    int typeChoice;
    std::cin >> typeChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    switch(typeChoice) {
        case 1: config.potType = "Clay"; break;
        case 2: config.potType = "Plastic"; break;
        case 3: config.potType = "Metal"; break;
        case 4: config.potType = "Glass"; break;
        case 5: config.potType = "Wooden"; break;
        default: config.potType = "Plastic";
    }
    
    // Select size
    std::cout << "\n    " << ANSI_CYAN << "Size Options:" << ANSI_RESET << "\n";
    std::cout << "    1. Small    2. Medium    3. Large\n";
    std::cout << "    " << ANSI_YELLOW << "➤ Select size (1-3): " << ANSI_RESET;
    int sizeChoice;
    std::cin >> sizeChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    switch(sizeChoice) {
        case 1: config.size = "Small"; break;
        case 2: config.size = "Medium"; break;
        case 3: config.size = "Large"; break;
        default: config.size = "Medium";
    }
    
    // Select shape
    std::cout << "\n    " << ANSI_CYAN << "Shape Options:" << ANSI_RESET << "\n";
    std::cout << "    1. Round    2. Square    3. Oval\n";
    std::cout << "    " << ANSI_YELLOW << "➤ Select shape (1-3): " << ANSI_RESET;
    int shapeChoice;
    std::cin >> shapeChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    switch(shapeChoice) {
        case 1: config.shape = "Round"; break;
        case 2: config.shape = "Square"; break;
        case 3: config.shape = "Oval"; break;
        default: config.shape = "Round";
    }
    
    // Drainage option
    std::cout << "\n    " << ANSI_CYAN << "Drainage:" << ANSI_RESET << "\n";
    std::cout << "    1. With drainage holes    2. No drainage\n";
    std::cout << "    " << ANSI_YELLOW << "➤ Select drainage (1-2): " << ANSI_RESET;
    int drainChoice;
    std::cin >> drainChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    config.hasDrainage = (drainChoice == 1);
    
    return config;
}

Pot* OrderUIFacade::createCustomizedPot(const PotCustomization& config) {
    PotFactory* factory = nullptr;
    
    if (config.potType == "Clay") {
        factory = new ClayPotFactory();
    } else if (config.potType == "Plastic") {
        factory = new PlasticPotFactory();
    } else if (config.potType == "Metal") {
        factory = new MetalPotFactory();
    } else if (config.potType == "Glass") {
        factory = new GlassPotFactory();
    } else if (config.potType == "Wooden") {
        factory = new WoodenPotFactory();
    } else {
        factory = new PlasticPotFactory(); // Default
    }
    
    Pot* pot = factory->createPot(config.size, config.shape, config.hasDrainage);
    delete factory;
    
    return pot;
}

bool OrderUIFacade::addColorToPot(Pot*& pot, PotCustomization& config) {
    std::cout << "\n    " << ANSI_MAGENTA << "Available Colors:" << ANSI_RESET << "\n";
    std::cout << "    " << ANSI_CYAN << "Standard (R3.00):" << ANSI_RESET << "\n";
    std::cout << "    1. White       2. Black      3. Red         4. Blue\n";
    std::cout << "    5. Green       6. Yellow     7. Purple      8. Pink\n";
    std::cout << "    9. Orange     10. Brown     11. Terracotta 12. Sage Green\n\n";
    std::cout << "    " << ANSI_YELLOW << "Metallic (R5.00):" << ANSI_RESET << "\n";
    std::cout << "   13. Gold       14. Silver    15. Copper     16. Bronze\n";
    
    std::cout << "\n    " << ANSI_YELLOW << "➤ Select color (1-16): " << ANSI_RESET;
    int colorChoice;
    std::cin >> colorChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::string colorName;
    switch(colorChoice) {
        case 1: colorName = "White"; break;
        case 2: colorName = "Black"; break;
        case 3: colorName = "Red"; break;
        case 4: colorName = "Blue"; break;
        case 5: colorName = "Green"; break;
        case 6: colorName = "Yellow"; break;
        case 7: colorName = "Purple"; break;
        case 8: colorName = "Pink"; break;
        case 9: colorName = "Orange"; break;
        case 10: colorName = "Brown"; break;
        case 11: colorName = "Terracotta"; break;
        case 12: colorName = "Sage Green"; break;
        case 13: colorName = "Gold"; break;
        case 14: colorName = "Silver"; break;
        case 15: colorName = "Copper"; break;
        case 16: colorName = "Bronze"; break;
        default: colorName = "White";
    }
    
    config.color = colorName;
    ColorDecorator* decorated = new ColorDecorator(pot, colorName);
    config.totalPrice = decorated->getPrice();
    pot = decorated;
    
    std::cout << "    " << ANSI_GREEN << "✓ Added " << colorName << " color!\n" << ANSI_RESET;
    return true;
}

bool OrderUIFacade::addFinishToPot(Pot*& pot, PotCustomization& config) {
    std::cout << "\n    " << ANSI_MAGENTA << "Available Finishes:" << ANSI_RESET << "\n";
    std::cout << "    1. Glossy (R2.00)     - Shiny, easy to clean\n";
    std::cout << "    2. Matte (R2.00)      - Smooth, non-reflective\n";
    std::cout << "    3. Textured (R3.00)   - Rough surface, better grip\n";
    std::cout << "    4. Glazed (R4.00)     - Ceramic coating, premium\n";
    std::cout << "    5. Weathered (R4.00)  - Aged, vintage look\n";
    
    std::cout << "\n    " << ANSI_YELLOW << "➤ Select finish (1-5): " << ANSI_RESET;
    int finishChoice;
    std::cin >> finishChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::string finishName;
    switch(finishChoice) {
        case 1: finishName = "Glossy"; break;
        case 2: finishName = "Matte"; break;
        case 3: finishName = "Textured"; break;
        case 4: finishName = "Glazed"; break;
        case 5: finishName = "Weathered"; break;
        default: finishName = "Matte";
    }
    
    config.finish = finishName;
    FinishDecorator* decorated = new FinishDecorator(pot, finishName);
    config.totalPrice = decorated->getPrice();
    pot = decorated;
    
    std::cout << "    " << ANSI_GREEN << "✓ Added " << finishName << " finish!\n" << ANSI_RESET;
    return true;
}

bool OrderUIFacade::addPatternToPot(Pot*& pot, PotCustomization& config) {
    std::cout << "\n    " << ANSI_MAGENTA << "Available Patterns:" << ANSI_RESET << "\n";
    std::cout << "    " << ANSI_CYAN << "Geometric (R4.00):" << ANSI_RESET << "\n";
    std::cout << "    1. Stripes     2. Polka Dots    3. Chevron    4. Hexagons\n\n";
    std::cout << "    " << ANSI_CYAN << "Floral/Nature (R6.00):" << ANSI_RESET << "\n";
    std::cout << "    5. Floral Design    6. Botanical Leaves    7. Vine Pattern\n\n";
    std::cout << "    " << ANSI_CYAN << "Artistic:" << ANSI_RESET << "\n";
    std::cout << "    8. Abstract (R4.00)    9. Hand-Painted (R8.00)\n";
    
    std::cout << "\n    " << ANSI_YELLOW << "➤ Select pattern (1-9): " << ANSI_RESET;
    int patternChoice;
    std::cin >> patternChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::string patternName;
    switch(patternChoice) {
        case 1: patternName = "Stripes"; break;
        case 2: patternName = "Polka Dots"; break;
        case 3: patternName = "Chevron"; break;
        case 4: patternName = "Hexagons"; break;
        case 5: patternName = "Floral Design"; break;
        case 6: patternName = "Botanical Leaves"; break;
        case 7: patternName = "Vine Pattern"; break;
        case 8: patternName = "Abstract"; break;
        case 9: patternName = "Hand-Painted Art"; break;
        default: patternName = "Stripes";
    }
    
    config.pattern = patternName;
    PatternDecorator* decorated = new PatternDecorator(pot, patternName);
    config.totalPrice = decorated->getPrice();
    pot = decorated;
    
    std::cout << "    " << ANSI_GREEN << "✓ Added " << patternName << " pattern!\n" << ANSI_RESET;
    return true;
}

bool OrderUIFacade::addCustomizedPotToOrder(int quantity) {
    if (!customer) {
        TerminalUI::printError("No customer associated with UI facade.");
        return false;
    }
    
    std::cout << "\n" << ANSI_MAGENTA << ANSI_BOLD;
    std::cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
    std::cout << "    ║                    🏺 CUSTOMIZE YOUR POT 🏺                      ║\n";
    std::cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
    std::cout << ANSI_RESET << "\n";
    
    // Configure pot base properties
    PotCustomization potConfig = interactivePotConfiguration();
    
    // Create the base pot using factory
    Pot* pot = createCustomizedPot(potConfig);
    
    // Interactive customization loop
    bool customizing = true;
    while (customizing) {
        displayPotConfigSummary(potConfig);
        displayPotCustomizationOptions();
        
        int customChoice;
        std::cin >> customChoice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch(customChoice) {
            case 1:
                addColorToPot(pot, potConfig);
                break;
            case 2:
                addFinishToPot(pot, potConfig);
                break;
            case 3:
                addPatternToPot(pot, potConfig);
                break;
            case 4:
                customizing = false;
                break;
            default:
                std::cout << ANSI_RED << "    ✗ Invalid choice!\n" << ANSI_RESET;
        }
    }
    
    // Use customer's builder to add pot to order through proper builder method
    ConcreteOrderBuilder* builder = customer->getOrderBuilder();
    if (builder) {
        std::string potDesc = getPotDescription(potConfig);
        
        // Use the builder method instead of creating items directly
        builder->buildCustomizedPot(potDesc, potConfig.totalPrice, quantity);
        
        delete pot;  // Clean up pot object after using it for price calculation
        
        std::cout << "\n    " << ANSI_GREEN << "✓ Added " << quantity << "x " << potDesc << " to cart!\n" << ANSI_RESET;
        std::cout << "    Price per pot: R" << std::fixed << std::setprecision(2) << potConfig.totalPrice << "\n";
        
        customer->notifyInteraction("PotPurchase", "Added " + std::to_string(quantity) + "x customized pot");
        return true;
    }
    
    delete pot;
    TerminalUI::printError("Unable to add pot to order.");
    return false;
}

bool OrderUIFacade::addPlantWithCustomizedPot(int plantIndex, int quantity) {
    if (!customer) {
        TerminalUI::printError("No customer associated with UI facade.");
        return false;
    }
    
    std::vector<PlantProduct*> plants = getAvailablePlantsFromInventory();
    
    // Validate plant selection
    if (plantIndex < 1 || plantIndex > static_cast<int>(plants.size())) {
        TerminalUI::printError("Invalid plant selection.");
        return false;
    }
    
    if (quantity < 1) {
        TerminalUI::printError("Quantity must be at least 1.");
        return false;
    }
    
    PlantProduct* selectedPlant = plants[plantIndex-1];
    std::string selectedPlantType = selectedPlant->getProfile()->getSpeciesName();
    
    // Get price from profile property, default to 25.99 if not set
    std::string priceStr = selectedPlant->getProfile()->getProperty("price");
    double plantPrice = priceStr.empty() ? 25.99 : std::stod(priceStr);
    
    std::cout << "\n" << ANSI_GREEN << ANSI_BOLD;
    std::cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
    std::cout << "    ║               🌱 PLANT + POT COMBINATION 🏺                      ║\n";
    std::cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
    std::cout << ANSI_RESET << "\n";
    
    std::cout << "    Selected Plant: " << ANSI_BOLD << selectedPlantType << ANSI_RESET 
             << " (R" << std::fixed << std::setprecision(2) << plantPrice << " each)\n";
    std::cout << "    Quantity: " << quantity << "\n\n";
    
    // Configure pot
    std::cout << ANSI_CYAN << "    Now let's customize the pot for your plant(s):\n" << ANSI_RESET;
    PotCustomization potConfig = interactivePotConfiguration();
    
    // Create the base pot using factory
    Pot* pot = createCustomizedPot(potConfig);
    
    // Interactive customization loop
    bool customizing = true;
    while (customizing) {
        displayPotConfigSummary(potConfig);
        displayPotCustomizationOptions();
        
        int customChoice;
        std::cin >> customChoice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch(customChoice) {
            case 1:
                addColorToPot(pot, potConfig);
                break;
            case 2:
                addFinishToPot(pot, potConfig);
                break;
            case 3:
                addPatternToPot(pot, potConfig);
                break;
            case 4:
                customizing = false;
                break;
            default:
                std::cout << ANSI_RED << "    ✗ Invalid choice!\n" << ANSI_RESET;
        }
    }
    
    // Use customer's builder to add plant with pot through proper builder method using PlantProduct data
    ConcreteOrderBuilder* builder = customer->getOrderBuilder();
    if (builder) {
        std::string potDesc = getPotDescription(potConfig);
        
        // Use the new builder method that accepts PlantProduct for accurate pricing
        builder->buildPlantWithCustomizedPotFromProduct(selectedPlant, potDesc, potConfig.totalPrice, quantity);
        
        delete pot;  // Clean up pot object
        
        double totalComboPrice = plantPrice + potConfig.totalPrice;
        std::cout << "\n    " << ANSI_GREEN << ANSI_BOLD << "✓ Added " << quantity << "x " 
                 << selectedPlantType << " + " << potDesc << " to cart!\n" << ANSI_RESET;
        std::cout << "    Price per combo: R" << std::fixed << std::setprecision(2) << totalComboPrice << "\n";
        
        customer->notifyInteraction("PlantWithPot", "Added " + std::to_string(quantity) + "x " + selectedPlantType + " with customized pot");
        return true;
    }
    
    delete pot;
    TerminalUI::printError("Unable to add plant with pot to order.");
    return false;
}
