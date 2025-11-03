#ifndef METALPOTFACTORY_H
#define METALPOTFACTORY_H

#include "Pot.h"
#include "MetalPot.h"
#include "PotFactory.h"
/**
 * @class MetalPotFactory
 * @brief Concrete factory for creating MetalPot objects
 *
 * This class implements the PotFactory interface to create
 * instances of MetalPot with specified properties.
 */

class MetalPotFactory : public PotFactory
{
public: /**
         * @brief Creates a new metal pot
         * @param size The size of the metal pot
         * @param shape The shape of the metal pot
         * @param drainage Whether the metal pot has drainage holes
         * @return Pointer to a newly created MetalPot object
         */
    Pot *createPot(std::string size, std::string shape, bool drainage) override;
};

#endif