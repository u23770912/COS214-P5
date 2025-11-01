#ifndef CLAYPOTFACTORY_H
#define CLAYPOTFACTORY_H

#include "Pot.h"
#include "ClayPot.h"
#include "PotFactory.h"

/**
 * @class ClayPotFactory
 * @brief Concrete factory for creating ClayPot objects
 *
 * This class implements the PotFactory interface to create
 * instances of ClayPot with specified properties.
 */

class ClayPotFactory : public PotFactory
{
public:
    /**
     * @brief Creates a new clay pot
     * @param size The size of the clay pot
     * @param shape The shape of the clay pot
     * @param drainage Whether the clay pot has drainage holes
     * @return Pointer to a newly created ClayPot object
     */
    Pot *createPot(std::string size, std::string shape, bool drainage) override;
};

#endif