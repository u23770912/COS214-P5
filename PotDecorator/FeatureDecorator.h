#ifndef FEATUREDECORATOR_H
#define FEATUREDECORATOR_H

#include "PotDecorator.h"

/**
 * @class FeatureDecorator
 * @brief Concrete decorator that adds special functional features to pots
 * 
 * @details Provides premium functional features that enhance pot usability
 *          and provide additional benefits to plant care:
 * 
 * Available Features (Premium Tier):
 * 
 * Advanced Features:
 * - Self-Watering System (R25.00):
 *   * Built-in water reservoir with wicking system
 *   * Automatic water delivery to plant roots
 *   * Reduces watering frequency by up to 70%
 *   * Ideal for busy customers or vacation periods
 * 
 * - LED Lighting (R20.00):
 *   * Integrated decorative illumination
 *   * Solar-powered or battery-operated
 *   * Enhances night-time ambiance
 *   * Can provide supplemental grow light
 * 
 * - Wheeled Base (R15.00):
 *   * Heavy-duty casters with locking mechanism
 *   * Easy mobility for large/heavy pots
 *   * Ideal for rearranging or cleaning
 *   * Supports up to 100kg
 * 
 * - Insulated Walls (R12.00):
 *   * Double-wall construction
 *   * Temperature regulation (hot/cold protection)
 *   * Protects roots from extreme weather
 *   * Extends growing season
 * 
 * - UV Protection (R10.00):
 *   * UV-resistant coating
 *   * Prevents color fading
 *   * Protects pot material from sun damage
 *   * Extends pot lifespan by 3-5 years
 * 
 * - Drainage Tray (R8.00):
 *   * Matching removable saucer
 *   * Protects surfaces from water damage
 *   * Catches excess water for reabsorption
 *   * Easy to clean
 * 
 * Design Pattern: Decorator Pattern (Concrete Decorator)
 * 
 * Pricing Strategy:
 * - Utility features (Drainage Tray): R8.00
 * - Protection features (UV, Insulated): R10.00 - R12.00
 * - Mobility features (Wheeled Base): R15.00
 * - Advanced tech (LED, Self-Watering): R20.00 - R25.00
 * 
 * Usage Example:
 * @code
 * Pot* pot = new ClayPot("Large", "Round", true);
 * pot = new ColorDecorator(pot, "Terracotta Red");
 * pot = new FeatureDecorator(pot, "Self-Watering System");
 * pot = new FeatureDecorator(pot, "UV Protection");
 * // Result: R10 + R3 + R25 + R10 = R48.00
 * // Self-watering, UV-protected terracotta pot
 * @endcode
 * 
 * @note Multiple features can be stacked on the same pot
 * 
 * @see PotDecorator
 * @see ColorDecorator
 * 
 * @author Greenhouse Nursery Team
 * @version 2.0
 */
class FeatureDecorator : public PotDecorator {
private:
    /**
     * @brief The functional feature added to this pot
     * @details Examples: "Self-Watering System", "UV Protection", "LED Lighting"
     */
    std::string feature;
    
    /**
     * @brief Calculates price based on feature complexity and value
     * 
     * @param feature Feature name to price
     * @return Price in Rands (R8.00 to R25.00)
     * 
     * @details Pricing logic based on feature sophistication:
     * 
     * Premium Technology (R20.00 - R25.00):
     * - "Self-Watering System": R25.00
     *   Complex mechanism with reservoir and wicking system
     * - "LED Lighting": R20.00
     *   Electronic components and power system
     * 
     * Enhanced Utility (R12.00 - R15.00):
     * - "Wheeled Base": R15.00
     *   Heavy-duty hardware and assembly
     * - "Insulated Walls": R12.00
     *   Double-wall construction and materials
     * 
     * Protection Features (R8.00 - R10.00):
     * - "UV Protection": R10.00
     *   Special coating application
     * - "Drainage Tray": R8.00
     *   Additional component
     * 
     * Default:
     * - Other features: R5.00 (standard add-on)
     * 
     * @note Prices reflect material costs, labor, and value to customer
     */
    double getFeaturePrice(const std::string& feature) const;
    
public:
    /**
     * @brief Constructs a feature decorator for a pot
     * 
     * @param pot Pointer to the pot to enhance
     * @param feature Feature name (e.g., "Self-Watering", "UV Protection")
     * 
     * @details Automatically determines pricing based on feature complexity
     *          and passes it to the base PotDecorator constructor.
     * 
     * Feature Installation:
     * - Features are compatible with all pot materials
     * - Some features work best with specific pot sizes:
     *   * Self-Watering: Medium to Large pots
     *   * Wheeled Base: Large to Extra Large pots
     *   * LED Lighting: Any size
     * 
     * Feature Combinations:
     * Multiple features can be added to create premium pots:
     * - Self-Watering + UV Protection: Ultimate low-maintenance
     * - LED Lighting + Wheeled Base: Mobile decorative planter
     * - Insulated + Drainage Tray: Climate-controlled system
     * 
     * @note Takes ownership of the pot pointer
     * @warning Some feature combinations may require professional installation
     */
    FeatureDecorator(Pot* pot, const std::string& feature);
    
    /**
     * @brief Prints pot description including feature decoration
     * 
     * @details Output format:
     *          "[Previous decorations] + [Feature] Feature (R[price])"
     * 
     * Example outputs:
     * - "Clay Pot [...] + Self-Watering System Feature (R25.00)"
     * - "Plastic Pot [...] + Red Color (R3.00) + UV Protection Feature (R10.00)"
     * 
     * Multi-feature example:
     * "Clay Pot [...] + Terracotta Color (R3.00) + Glossy Finish (R2.00) 
     *  + Self-Watering System Feature (R25.00) + UV Protection Feature (R10.00)"
     * 
     * Implementation:
     * 1. Calls wrappedPot->print() to display base/previous decorations
     * 2. Appends feature information with calculated price
     * 
     * The chained output provides complete transparency about all
     * added features and their individual costs.
     * 
     * @note Does not print newline - enables decorator chaining
     */
    void print() override;
    
    /**
     * @brief Gets the feature type of this decorator
     * @return Feature name string
     * 
     * @details Useful for:
     *          - Inventory management and categorization
     *          - Feature popularity tracking
     *          - Warranty and maintenance records
     *          - Customer support and troubleshooting
     *          - Upselling recommendations
     */
    std::string getFeature() const { return feature; }
};

#endif // FEATUREDECORATOR_H