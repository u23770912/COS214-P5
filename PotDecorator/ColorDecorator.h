#ifndef COLORDECORATOR_H
#define COLORDECORATOR_H

#include "PotDecorator.h"

/**
 * @class ColorDecorator
 * @brief Concrete decorator that adds color customization to pots
 * 
 * @details Allows customers to choose from a variety of colors including:
 *          - Standard Colors (R3.00): White, Black, Red, Blue, Green, Yellow, 
 *            Purple, Pink, Orange, Brown, Terracotta, Sage Green
 *          - Metallic Colors (R5.00): Gold, Silver, Copper, Bronze
 * 
 * Design Pattern: Decorator Pattern (Concrete Decorator)
 * 
 * Pricing Strategy:
 * - Standard colors cost R3.00 additional
 * - Metallic finishes cost R5.00 additional (premium pricing)
 * 
 * The pricing is determined automatically by checking if the color
 * name contains metallic keywords (Gold, Silver, Copper, Bronze).
 * 
 * Usage Example:
 * @code
 * // Standard color
 * Pot* pot = new ClayPot("Medium", "Round", true);
 * pot = new ColorDecorator(pot, "Terracotta Red");
 * // Adds R3.00 to base price
 * 
 * // Metallic color
 * pot = new ColorDecorator(pot, "Gold");
 * // Adds R5.00 to current price
 * @endcode
 * 
 * @see PotDecorator
 * @see InventoryManager
 * 
 * @author Greenhouse Nursery Team
 * @version 2.0
 */
class ColorDecorator : public PotDecorator {
private:
    /**
     * @brief The color name applied to this pot
     * @details Examples: "Terracotta Red", "Ocean Blue", "Gold", "Silver"
     */
    std::string color;
    
    /**
     * @brief Determines if a color is metallic for pricing purposes
     * 
     * @param color Color name to check
     * @return true if color contains "Gold", "Silver", "Copper", or "Bronze"
     * @return false otherwise
     * 
     * @details Metallic colors cost R5.00 while standard colors cost R3.00.
     *          This method uses string matching to detect metallic keywords.
     * 
     * Implementation:
     * - Searches for "Gold", "Silver", "Copper", "Bronze" in color name
     * - Case-sensitive matching
     * - Returns true if any keyword is found
     */
    bool isMetallic(const std::string& color) const;
    
public:
    /**
     * @brief Constructs a color decorator for a pot
     * 
     * @param pot Pointer to the pot to colorize (base or already decorated)
     * @param color Color name (e.g., "Terracotta Red", "Ocean Blue", "Gold")
     * 
     * @details Automatically determines pricing based on color type:
     *          - Metallic colors: R5.00
     *          - Standard colors: R3.00
     * 
     * The constructor calls isMetallic() to determine the appropriate price
     * and passes it to the base PotDecorator constructor.
     * 
     * @note Takes ownership of the pot pointer
     */
    ColorDecorator(Pot* pot, const std::string& color);
    
    /**
     * @brief Prints pot description including color decoration
     * 
     * @details Output format:
     *          "[Previous decorations] + [Color] Color (R[price])"
     * 
     * Example outputs:
     * - "Clay Pot [...] + Terracotta Red Color (R3.00)"
     * - "Plastic Pot [...] + Gold Color (R5.00)"
     * 
     * Implementation:
     * 1. Calls wrappedPot->print() to display base/previous decorations
     * 2. Appends color information with price
     * 
     * @note Does not print newline - allows chaining with other decorators
     */
    void print() override;
    
    /**
     * @brief Gets the color of this decorator
     * @return Color name string
     * 
     * @details Useful for:
     *          - Displaying pot details in inventory
     *          - Filtering pots by color
     *          - Generating reports
     */
    std::string getColor() const { return color; }
};

#endif // COLORDECORATOR_H