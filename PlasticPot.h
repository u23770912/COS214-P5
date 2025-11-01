#ifndef PLASTICPOT_H
#define PLASTICPOT_H

#include "Pot.h"

/**
 * @class PlasticPot
 * @brief Represents a pot made of plastic material
 *
 * This class is a concrete implementation of the Pot base class,
 * specifically for pots made from plastic.
 */

class PlasticPot : public Pot
{
public:
    /**
     * @brief Constructs a PlasticPot object
     * @param size The size of the plastic pot
     * @param shape The shape of the plastic pot
     * @param drainage Whether the plastic pot has drainage holes
     */
    PlasticPot(std::string size, std::string shape, bool drainage);
    void print() override;
    std::string getPotType() const override;
};

#endif