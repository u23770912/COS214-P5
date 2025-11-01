#ifndef GLASSPOT_H
#define GLASSPOT_H

#include "Pot.h"

/**
 * @class GlassPot
 * @brief Represents a pot made of glass material
 *
 * This class is a concrete implementation of the Pot base class,
 * specifically for pots made from glass.
 */

class GlassPot : public Pot
{
public:
    /**
     * @brief Constructs a GlassPot object
     * @param size The size of the glass pot
     * @param shape The shape of the glass pot
     * @param drainage Whether the glass pot has drainage holes
     */

    GlassPot(std::string size, std::string shape, bool drainage);
    void print() override;
    std::string getPotType() const override;
};

#endif