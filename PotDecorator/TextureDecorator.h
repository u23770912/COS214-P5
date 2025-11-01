#ifndef TEXTUREDECORATOR_H
#define TEXTUREDECORATOR_H

#include "PotDecorator.h"

/**
 * @class TextureDecorator
 * @brief Concrete decorator that adds tactile surface texture to pots
 * 
 * @details Provides various surface textures that enhance both the visual
 *          and tactile experience of the pot:
 * 
 * Available Textures:
 * 
 * Premium Textures (R6.00):
 * - Embossed: Raised patterns pressed into surface
 *   * Creates 3D relief designs
 *   * Adds visual depth and shadows
 *   * Elegant, sophisticated appearance
 * 
 * - Woven: Basket-weave pattern
 *   * Mimics natural wicker or rattan
 *   * Rustic, organic aesthetic
 *   * Popular for cottage/farmhouse styles
 * 
 * Enhanced Textures (R4.00):
 * - Hammered: Metallic textured look
 *   * Hand-forged appearance
 *   * Multiple small indentations
 *   * Modern industrial style
 * 
 * - Ribbed: Vertical or horizontal grooves
 *   * Linear texture patterns
 *   * Better grip for handling
 *   * Contemporary clean lines
 * 
 * Standard Textures (R3.00):
 * - Rustic: Rough, natural unfinished look
 * - Sandy: Fine grain texture
 * - Smooth: Minimal texture (default)
 * 
 * Design Pattern: Decorator Pattern (Concrete Decorator)
 * 
 * Pricing Strategy:
 * - Complex artistic textures (Embossed, Woven): R6.00
 * - Enhanced surface patterns (Hammered, Ribbed): R4.00
 * - Basic textures (Rustic, Sandy, Smooth): R3.00
 * 
 * Texture Benefits:
 * - Improved grip and handling
 * - Enhanced visual interest
 * - Unique tactile experience
 * - Better integration with decor themes
 * - Can mask minor surface imperfections
 * 
 * Usage Example:
 * @code
 * Pot* pot = new ClayPot("Medium", "Round", true);
 * pot = new ColorDecorator(pot, "Clay Brown");
 * pot = new TextureDecorator(pot, "Embossed"); // Adds R6.00
 * // Result: Brown clay pot with embossed texture
 * @endcode
 * 
 * Design Recommendations:
 * - Textured pots pair well with matte finishes
 * - Consider texture visibility with chosen color
 * - Darker colors enhance shadow effects of texture
 * - Smooth textures best for glossy finishes
 * 
 * @see PotDecorator
 * @see FinishDecorator
 * @see ColorDecorator
 * 
 * @author Greenhouse Nursery Team
 * @version 2.0
 */
class TextureDecorator : public PotDecorator {
private:
    /**
     * @brief The surface texture applied to this pot
     * @details Examples: "Embossed", "Hammered", "Ribbed", "Woven", "Rustic"
     */
    std::string texture;
    
    /**
     * @brief Calculates price based on texture complexity and labor
     * 
     * @param texture Texture type name
     * @return Price in Rands (R3.00 to R6.00)
     * 
     * @details Pricing logic based on manufacturing complexity:
     * 
     * Premium Tier (R6.00):
     * - "Embossed": Requires custom molds and pressing
     * - "Woven": Labor-intensive pattern application
     * - These textures require specialized equipment and skill
     * 
     * Enhanced Tier (R4.00):
     * - "Hammered": Metallic texturing process
     * - "Ribbed": Requires special forming tools
     * - Moderate complexity in application
     * 
     * Standard Tier (R3.00):
     * - All other textures (Rustic, Sandy, Smooth, etc.)
     * - Basic texturing or minimal processing
     * - Standard manufacturing process
     * 
     * Implementation uses string comparison for easy expansion:
     * - Checks for premium textures first
     * - Then enhanced textures
     * - Defaults to standard pricing
     * 
     * @note Texture pricing considers both material and labor costs
     */
    double getTexturePrice(const std::string& texture) const;
    
public:
    /**
     * @brief Constructs a texture decorator for a pot
     * 
     * @param pot Pointer to the pot to texturize
     * @param texture Texture type (e.g., "Embossed", "Hammered", "Ribbed")
     * 
     * @details Automatically calculates pricing based on texture complexity
     *          and passes it to the base PotDecorator constructor.
     * 
     * Texture Application:
     * - Applied to pot surface during or after base manufacturing
     * - Can be combined with colors (applied before or after texture)
     * - Interacts with finish decorators:
     *   * Glossy finishes smooth out fine textures
     *   * Matte finishes preserve texture details
     *   * Textured finishes can enhance base texture
     * 
     * Texture Compatibility:
     * - All textures work on all pot materials
     * - Some materials showcase textures better:
     *   * Clay: Excellent for all textures
     *   * Plastic: Best for molded textures (Embossed, Ribbed)
     *   * Metal: Perfect for Hammered texture
     *   * Wood: Natural choice for Rustic texture
     * 
     * @note Takes ownership of the pot pointer
     * @warning Applying finish after texture may reduce texture visibility
     */
    TextureDecorator(Pot* pot, const std::string& texture);
    
    /**
     * @brief Prints pot description including texture decoration
     * 
     * @details Output format:
     *          "[Previous decorations] + [Texture] Texture (R[price])"
     * 
     * Example outputs:
     * - "Clay Pot [...] + Embossed Texture (R6.00)"
     * - "Metal Pot [...] + Silver Color (R5.00) + Hammered Texture (R4.00)"
     * 
     * Complex example:
     * "Clay Pot [Medium, Round, Drainage: Yes] 
     *  + Clay Brown Color (R3.00) 
     *  + Matte Finish (R2.00) 
     *  + Embossed Texture (R6.00)"
     * Total: R10 + R3 + R2 + R6 = R21.00
     * 
     * Implementation:
     * 1. Calls wrappedPot->print() to display base/previous decorations
     * 2. Appends texture information with calculated price
     * 
     * The complete output allows customers to see the full
     * customization journey and understand pricing breakdown.
     * 
     * @note Does not print newline - enables decorator chaining
     */
    void print() override;
    
    /**
     * @brief Gets the texture type of this decorator
     * @return Texture type name
     * 
     * @details Useful for:
     *          - Inventory categorization and search
     *          - Customer preference analysis
     *          - Matching with plant types and aesthetics
     *          - Quality control and manufacturing tracking
     *          - Reordering popular texture styles
     */
    std::string getTexture() const { return texture; }
};

#endif // TEXTUREDECORATOR_H
