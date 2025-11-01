#ifndef METALPOT_H
#define METALPOT_H

#include "Pot.h"

/**
 * @class MetalPot
 * @brief Represents a pot made of metal material
 *
 * This class is a concrete implementation of the Pot base class,
 * specifically for pots made from metal.
 */

class MetalPot : public Pot
{
public:
    /**
     * @brief Constructs a MetalPot object
     * @param size The size of the metal pot
     * @param shape The shape of the metal pot
     * @param drainage Whether the metal pot has drainage holes
     */
    MetalPot(std::string size, std::string shape, bool drainage);
    void print() override;
    std::string getPotType() const override;
};

#endif