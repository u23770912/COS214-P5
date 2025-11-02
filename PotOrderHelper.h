#ifndef POT_ORDER_HELPER_H
#define POT_ORDER_HELPER_H

#include <iostream>
#include <string>
#include <vector>
#include <limits>
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

/**
 * @file PotOrderHelper.h
 * @brief Helper functions for interactive pot customization and ordering
 * 
 * This file provides utility functions for:
 * - Displaying pot type catalogs
 * - Interactive pot customization with decorators
 * - Pot configuration selection (size, shape, drainage)
 * - Price calculation with all customizations
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

/**
 * @struct PotConfiguration
 * @brief Holds all configuration details for a customized pot
 */
struct PotConfiguration {
    std::string potType;      // "Clay", "Plastic", "Metal", "Glass", "Wooden"
    std::string size;         // "Small", "Medium", "Large"
    std::string shape;        // "Round", "Square", "Oval"
    bool hasDrainage;         // true/false
    std::string color;        // Color name (optional)
    std::string finish;       // Finish type (optional)
    std::string pattern;      // Pattern type (optional)
    double totalPrice;        // Final calculated price
    
    PotConfiguration() : potType(""), size(""), shape(""), hasDrainage(true), 
                        color(""), finish(""), pattern(""), totalPrice(10.0) {}
};

/**
 * @brief Display all available pot types with base prices
 */
inline void displayPotTypes() {
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

/**
 * @brief Display customization options menu
 */
inline void displayCustomizationMenu() {
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

/**
 * @brief Create pot factory based on type selection
 */
inline PotFactory* createPotFactory(int typeChoice) {
    switch(typeChoice) {
        case 1: return new ClayPotFactory();
        case 2: return new PlasticPotFactory();
        case 3: return new MetalPotFactory();
        case 4: return new GlassPotFactory();
        case 5: return new WoodenPotFactory();
        default: return new PlasticPotFactory();
    }
}

/**
 * @brief Get pot type name from choice
 */
inline std::string getPotTypeName(int typeChoice) {
    switch(typeChoice) {
        case 1: return "Clay";
        case 2: return "Plastic";
        case 3: return "Metal";
        case 4: return "Glass";
        case 5: return "Wooden";
        default: return "Plastic";
    }
}

/**
 * @brief Interactive pot configuration (size, shape, drainage)
 */
inline PotConfiguration configurePot() {
    PotConfiguration config;
    
    // Select pot type
    displayPotTypes();
    std::cout << "    " << ANSI_YELLOW << "➤ Select pot type (1-5): " << ANSI_RESET;
    int typeChoice;
    std::cin >> typeChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (typeChoice < 1 || typeChoice > 5) {
        typeChoice = 2; // Default to Plastic
    }
    config.potType = getPotTypeName(typeChoice);
    
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

/**
 * @brief Add color customization
 */
inline Pot* addColorCustomization(Pot* pot, PotConfiguration& config) {
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
    
    std::cout << "    " << ANSI_GREEN << "✓ Added " << colorName << " color!\n" << ANSI_RESET;
    return decorated;
}

/**
 * @brief Add finish customization
 */
inline Pot* addFinishCustomization(Pot* pot, PotConfiguration& config) {
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
    
    std::cout << "    " << ANSI_GREEN << "✓ Added " << finishName << " finish!\n" << ANSI_RESET;
    return decorated;
}

/**
 * @brief Add pattern customization
 */
inline Pot* addPatternCustomization(Pot* pot, PotConfiguration& config) {
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
    
    std::cout << "    " << ANSI_GREEN << "✓ Added " << patternName << " pattern!\n" << ANSI_RESET;
    return decorated;
}

/**
 * @brief Display current pot configuration summary
 */
inline void displayPotSummary(const PotConfiguration& config) {
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

/**
 * @brief Create a customized pot string description for order display
 */
inline std::string getPotDescription(const PotConfiguration& config) {
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

#endif // POT_ORDER_HELPER_H
