#ifndef CLAYPOT_H
#define CLAYPOT_H

#include "Pot.h"

/**
 * @class ClayPot
 * @brief Represents a pot made of clay material
 *
 * This class is a concrete implementation of the Pot base class,
 * specifically for pots made from clay.
 */

class ClayPot : public Pot
{
public:
    /**
     * @brief Constructs a ClayPot object
     * @param size The size of the clay pot
     * @param shape The shape of the clay pot
     * @param drainage Whether the clay pot has drainage holes
     */
    ClayPot(std::string size, std::string shape, bool drainage);
    void print() override;
    std::string getPotType() const override;
};

#endif