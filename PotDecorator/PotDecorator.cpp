#ifndef POTDECORATOR_H
#define POTDECORATOR_H

#include "Pot.h"
#include <string>

/**
 * @class PotDecorator
 * @brief Abstract decorator base class for pot customization (Decorator Pattern)
 * 
 * @details This class implements the Decorator Pattern to enable dynamic addition
 *          of features to pots without modifying existing pot classes. It wraps
 *          a Pot object and delegates operations to it while adding extra behavior.
 * 
 * Design Pattern: Decorator Pattern
 * - Component: Pot (base interface)
 * - Concrete Components: ClayPot, PlasticPot, WoodenPot, MetalPot
 * - Decorator: PotDecorator (this class)
 * - Concrete Decorators: ColorDecorator, FinishDecorator, etc.
 * 
 * Key Features:
 * - Wraps another Pot (base or decorated)
 * - Adds decoration-specific pricing
 * - Maintains pot interface (polymorphism)
 * - Enables decorator chaining
 * - Automatic cleanup of decorator chain
 * 
 * Client Relationship:
 * The InventoryManager acts as the CLIENT in the Decorator Pattern.
 * It stores Pot* pointers which can be:
 * - Base pots (ClayPot, PlasticPot, etc.)
 * - Decorated pots (with colors, finishes, patterns, features)
 * 
 * Usage Example:
 * @code
 * // Create base pot
 * Pot* pot = new ClayPot("Medium", "Round", true);
 * 
 * // Add decorations (chain decorators)
 * pot = new ColorDecorator(pot, "Terracotta Red");
 * pot = new FinishDecorator(pot, "Glossy");
 * 
 * // CLIENT (InventoryManager) stores it
 * InventoryManager& inventory = InventoryManager::getInstance();
 * inventory.addCustomPot(pot);
 * 
 * // Get total price (base + all decorations)
 * PotDecorator* dec = dynamic_cast<PotDecorator*>(pot);
 * double price = dec->getPrice(); // R15.00
 * 
 * // Cleanup (deletes entire chain)
 * delete pot;
 * @endcode
 * 
 * @see ColorDecorator
 * @see FinishDecorator
 * @see PatternDecorator
 * @see FeatureDecorator
 * @see TextureDecorator
 * @see InventoryManager
 * 
 * @author Greenhouse Nursery Team
 * @version 2.0
 */
class PotDecorator : public Pot {
protected:
    /**
     * @brief Pointer to the pot being decorated
     * @details Can point to a base pot or another decorator (decorator chain).
     *          This enables stacking multiple decorations on a single pot.
     */
    Pot* wrappedPot;
    
    /**
     * @brief Additional price for this specific decoration
     * @details Each decorator adds its own cost. Total price is calculated
     *          by summing base pot price + all decorator prices in the chain.
     */
    double decorationPrice;

public:
    /**
     * @brief Constructs a decorator wrapping an existing pot
     * 
     * @param pot Pointer to the pot to decorate (base or already decorated)
     * @param price Additional cost for this decoration in Rands
     * 
     * @details Takes ownership of the wrapped pot. When this decorator is
     *          deleted, it will delete the wrapped pot (chain cleanup).
     * 
     * @note The wrapped pot must remain valid for the lifetime of this decorator
     */
    PotDecorator(Pot* pot, double price = 0.0);
    
    /**
     * @brief Virtual destructor - cleans up entire decorator chain
     * 
     * @details Deletes the wrapped pot, which triggers recursive cleanup if
     *          the wrapped pot is also a decorator. This ensures the entire
     *          decorator chain is properly cleaned up with a single delete.
     * 
     * Example:
     * @code
     * Pot* pot = new ClayPot(...);
     * pot = new ColorDecorator(pot, ...);
     * pot = new FinishDecorator(pot, ...);
     * delete pot; // Cleans up FinishDecorator -> ColorDecorator -> ClayPot
     * @endcode
     */
    virtual ~PotDecorator();
    
    /**
     * @brief Calculates total price including all decorations
     * 
     * @return Total price in Rands (base pot + all decorator prices)
     * 
     * @details Recursively traverses the decorator chain to sum all prices:
     *          1. Checks if wrapped pot is also a decorator
     *          2. If yes, adds this price + wrapped decorator's price (recursive)
     *          3. If no (base pot), adds this price + base pot price (R10.00)
     * 
     * Example:
     * @code
     * ClayPot (R10) + Color (R3) + Finish (R2) + Feature (R25) = R40.00
     * @endcode
     * 
     * @note Base pot price is hardcoded as R10.00
     * @see decorationPrice
     */
    virtual double getPrice() const;
    
    /**
     * @brief Pure virtual method to print pot description
     * 
     * @details Concrete decorators must implement this to:
     *          1. Call wrappedPot->print() to print base/previous decorations
     *          2. Add their own decoration description
     * 
     * This creates a chain of print calls that displays all decorations:
     * "Base Pot [...] + Color [...] + Finish [...] + Pattern [...]"
     * 
     * @note Must be overridden by all concrete decorators
     */
    virtual void print() override = 0;
};

#endif // POTDECORATOR_H