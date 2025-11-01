#ifndef GLASSPOTFACTORY_H
#define GLASSPOTFACTORY_H

#include "Pot.h"
#include "GlassPot.h"
#include "PotFactory.h"

/**
 * @class GlassPotFactory
 * @brief Concrete factory for creating GlassPot objects
 *
 * This class implements the PotFactory interface to create
 * instances of GlassPot with specified properties.
 */

class GlassPotFactory : public PotFactory
{
public:
    /**
     * @brief Creates a new glass pot
     * @param size The size of the glass pot
     * @param shape The shape of the glass pot
     * @param drainage Whether the glass pot has drainage holes
     * @return Pointer to a newly created GlassPot object
     */
    Pot *createPot(std::string size, std::string shape, bool drainage) override;
};

#endif