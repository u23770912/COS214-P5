#ifndef WOODENPOTFACTORY_H
#define WOODENPOTFACTORY_H

#include "Pot.h"
#include "WoodenPot.h"
#include "PotFactory.h"
/**
 * @class WoodenPotFactory
 * @brief Concrete factory for creating WoodenPot objects
 *
 * This class implements the PotFactory interface to create
 * instances of WoodenPot with specified properties.
 */

class WoodenPotFactory : public PotFactory
{
public:
    /**
     * @brief Creates a new wooden pot
     * @param size The size of the wooden pot
     * @param shape The shape of the wooden pot
     * @param drainage Whether the wooden pot has drainage holes
     * @return Pointer to a newly created WoodenPot object
     */
    Pot *createPot(std::string size, std::string shape, bool drainage) override;
};

#endif