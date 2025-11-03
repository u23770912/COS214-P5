#ifndef FINISHDECORATOR_H
#define FINISHDECORATOR_H

#include "PotDecorator.h"

/**
 * @class FinishDecorator
 * @brief Concrete decorator that adds surface finish customization to pots
 * 
 * @details Provides different surface finish options that affect both aesthetics
 *          and functionality of the pot:
 * 
 * Available Finishes:
 * - Glossy (R2.00): Shiny, reflective surface - easy to clean, modern look
 * - Matte (R2.00): Non-reflective, smooth surface - contemporary aesthetic
 * - Textured (R3.00): Rough, tactile surface - better grip, rustic appearance
 * - Glazed (R4.00): Ceramic-style coating - premium look, enhanced durability
 * - Weathered (R4.00): Aged, vintage appearance - artistic, distressed finish
 * 
 * Design Pattern: Decorator Pattern (Concrete Decorator)
 * 
 * Pricing Tiers:
 * - Basic finishes (Glossy, Matte): R2.00
 * - Enhanced finish (Textured): R3.00
 * - Premium finishes (Glazed, Weathered): R4.00
 * 
 * Usage Example:
 * @code
 * Pot* pot = new ClayPot("Medium", "Round", true);
 * pot = new ColorDecorator(pot, "Terracotta Red");
 * pot = new FinishDecorator(pot, "Glossy"); // Adds R2.00
 * // Result: Shiny terracotta red clay pot
 * @endcode
 * 
 * @see PotDecorator
 * @see ColorDecorator
 * 
 * @author Greenhouse Nursery Team
 * @version 2.0
 */
class FinishDecorator : public PotDecorator {
private:
    /**
     * @brief The surface finish type applied to this pot
     * @details Examples: "Glossy", "Matte", "Textured", "Glazed", "Weathered"
     */
    std::string finish;
    
    /**
     * @brief Calculates price based on finish complexity
     * 
     * @param finish Finish type name
     * @return Price in Rands (R2.00 to R4.00)
     * 
     * @details Pricing logic:
     *          - "Glazed" or "Weathered": R4.00 (premium, complex finishes)
     *          - "Textured": R3.00 (enhanced finish)
     *          - Others (Glossy, Matte): R2.00 (standard finishes)
     * 
     * Implementation uses string comparison for flexibility in adding
     * new finish types without modifying the core logic.
     */
    double getFinishPrice(const std::string& finish) const;
    
public:
    /**
     * @brief Constructs a finish decorator for a pot
     * 
     * @param pot Pointer to the pot to apply finish to
     * @param finish Finish type (e.g., "Glossy", "Matte", "Textured")
     * 
     * @details Automatically calculates appropriate pricing based on finish
     *          complexity and passes it to base PotDecorator constructor.
     * 
     * The finish is applied regardless of pot material or previous decorations,
     * allowing combinations like:
     * - "Glossy" finish on colored pots
     * - "Weathered" finish on patterned pots
     * 
     * @note Takes ownership of the pot pointer
     */
    FinishDecorator(Pot* pot, const std::string& finish);
    
    /**
     * @brief Prints pot description including finish decoration
     * 
     * @details Output format:
     *          "[Previous decorations] + [Finish] Finish (R[price])"
     * 
     * Example outputs:
     * - "Clay Pot [...] + Terracotta Red Color (R3.00) + Glossy Finish (R2.00)"
     * - "Plastic Pot [...] + Glazed Finish (R4.00)"
     * 
     * Implementation:
     * 1. Calls wrappedPot->print() for base/previous decorations
     * 2. Appends finish information with calculated price
     * 
     * @note Does not print newline - allows decorator chaining
     */
    void print() override;
    
    /**
     * @brief Gets the finish type of this decorator
     * @return Finish type name
     * 
     * @details Useful for:
     *          - Inventory management and filtering
     *          - Display in product catalogs
     *          - Customer preference tracking
     */
    std::string getFinish() const { return finish; }
};

#endif // FINISHDECORATOR_H