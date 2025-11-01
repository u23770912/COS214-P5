#ifndef PATTERNDECORATOR_H
#define PATTERNDECORATOR_H

#include "PotDecorator.h"

/**
 * @class PatternDecorator
 * @brief Concrete decorator that adds decorative patterns to pots
 * 
 * @details Offers a wide variety of visual patterns to enhance pot aesthetics:
 * 
 * Pattern Categories:
 * 
 * Geometric Patterns (R4.00):
 * - Stripes: Vertical or horizontal lines
 * - Polka Dots: Circular spots pattern
 * - Chevron: V-shaped zigzag pattern
 * - Hexagons: Honeycomb-style pattern
 * 
 * Floral & Nature Patterns (R6.00):
 * - Floral Design: Hand-drawn flower motifs
 * - Botanical Leaves: Leaf and vine patterns
 * - Vine Pattern: Climbing plant designs
 * - Garden Scene: Comprehensive nature artwork
 * 
 * Artistic Patterns:
 * - Abstract (R4.00): Swirls, Marbled, Ombre effects
 * - Hand-Painted (R8.00): Custom artistic designs (premium)
 * 
 * Design Pattern: Decorator Pattern (Concrete Decorator)
 * 
 * Pricing Strategy:
 * - Simple geometric patterns: R4.00
 * - Nature/Floral patterns: R6.00 (more detailed)
 * - Hand-painted designs: R8.00 (premium, artisan work)
 * 
 * Usage Example:
 * @code
 * Pot* pot = new ClayPot("Large", "Round", true);
 * pot = new ColorDecorator(pot, "Classic White");
 * pot = new FinishDecorator(pot, "Matte");
 * pot = new PatternDecorator(pot, "Floral Design"); // Adds R6.00
 * // Result: White matte pot with floral pattern
 * @endcode
 * 
 * @see PotDecorator
 * @see ColorDecorator
 * @see FinishDecorator
 * 
 * @author Greenhouse Nursery Team
 * @version 2.0
 */
class PatternDecorator : public PotDecorator {
private:
    /**
     * @brief The decorative pattern applied to this pot
     * @details Examples: "Floral Design", "Stripes", "Hand-Painted Art"
     */
    std::string pattern;
    
    /**
     * @brief Calculates price based on pattern complexity and detail
     * 
     * @param pattern Pattern name to price
     * @return Price in Rands (R4.00 to R8.00)
     * 
     * @details Pricing logic based on artistic complexity:
     * 
     * Premium Tier (R8.00):
     * - "Hand-Painted" patterns: Requires skilled artisan work
     * 
     * Enhanced Tier (R6.00):
     * - "Floral" or "Botanical" patterns: Detailed nature designs
     * - Requires precision and multiple colors
     * 
     * Standard Tier (R4.00):
     * - Geometric patterns: Simpler, repeating designs
     * - All other patterns not in premium/enhanced tiers
     * 
     * Implementation uses substring matching for flexibility:
     * - Checks for "Hand-Painted" first (highest price)
     * - Then checks for "Floral" or "Botanical" (mid price)
     * - Defaults to standard pricing
     * 
     * @note String matching is case-sensitive
     */
    double getPatternPrice(const std::string& pattern) const;
    
public:
    /**
     * @brief Constructs a pattern decorator for a pot
     * 
     * @param pot Pointer to the pot to add pattern to
     * @param pattern Pattern name (e.g., "Floral Design", "Stripes")
     * 
     * @details Automatically determines pricing based on pattern complexity
     *          and passes it to the base PotDecorator constructor.
     * 
     * Pattern Application:
     * - Can be applied to any colored or finished pot
     * - Pattern visibility depends on base color and finish
     * - Recommended to apply after color but before texture
     * 
     * Design Consideration:
     * Patterns work best on:
     * - Light colored pots (better contrast)
     * - Smooth or glossy finishes (pattern clarity)
     * - Larger pot sizes (more visible detail)
     * 
     * @note Takes ownership of the pot pointer
     */
    PatternDecorator(Pot* pot, const std::string& pattern);
    
    /**
     * @brief Prints pot description including pattern decoration
     * 
     * @details Output format:
     *          "[Previous decorations] + [Pattern] Pattern (R[price])"
     * 
     * Example outputs:
     * - "Clay Pot [...] + White Color (R3.00) + Floral Design Pattern (R6.00)"
     * - "Plastic Pot [...] + Hand-Painted Art Pattern (R8.00)"
     * 
     * Implementation:
     * 1. Calls wrappedPot->print() to display base/previous decorations
     * 2. Appends pattern information with calculated price
     * 
     * The chained output shows the complete customization history,
     * allowing customers to see all applied decorations at once.
     * 
     * @note Does not print newline - enables decorator chaining
     */
    void print() override;
    
    /**
     * @brief Gets the pattern type of this decorator
     * @return Pattern name string
     * 
     * @details Useful for:
     *          - Inventory categorization and filtering
     *          - Customer preference analytics
     *          - Reordering popular patterns
     *          - Seasonal pattern recommendations
     */
    std::string getPattern() const { return pattern; }
};

#endif // PATTERNDECORATOR_H