#ifndef PLASTICPOTFACTORY_H
#define PLASTICPOTFACTORY_H

#include "Pot.h"
#include "PlasticPot.h"
#include "PotFactory.h"
/**
 * @class PlasticPotFactory
 * @brief Concrete factory for creating PlasticPot objects
 *
 * This class implements the PotFactory interface to create
 * instances of PlasticPot with specified properties.
 */
class PlasticPotFactory : public PotFactory
{
public:
    /**
     * @brief Creates a new plastic pot
     * @param size The size of the plastic pot
     * @param shape The shape of the plastic pot
     * @param drainage Whether the plastic pot has drainage holes
     * @return Pointer to a newly created PlasticPot object
     */
    Pot *createPot(std::string size, std::string shape, bool drainage) override;
};

#endif