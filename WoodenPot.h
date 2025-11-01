#ifndef WOODENPOT_H
#define WOODENPOT_H

#include "Pot.h"

/**
 * @class WoodenPot
 * @brief Represents a pot made of wood material
 *
 * This class is a concrete implementation of the Pot base class,
 * specifically for pots made from wooden.
 */

class WoodenPot : public Pot
{
public:
    /**
     * @brief Constructs a WoodenPot object
     * @param size The size of the wooden pot
     * @param shape The shape of the wooden pot
     * @param drainage Whether the wooden pot has drainage holes
     */
    WoodenPot(std::string size, std::string shape, bool drainage);
    void print() override;
    std::string getPotType() const override;
};

#endif