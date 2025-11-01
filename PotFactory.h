#ifndef POTFACTORY_H
#define POTFACTORY_H

#include "Pot.h"

/**
 * @class PotFactory
 * @brief Abstract factory class for creating different types of pots
 *
 * This class defines the interface for concrete pot factories.
 * Each derived factory creates a specific type of pot (clay, glass, metal, etc.).
 */

class PotFactory
{
public:
    virtual ~PotFactory() {}

    /**
     * @brief Pure virtual function to create a pot
     * @param size The size of the pot to create
     * @param shape The shape of the pot to create
     * @param drainage Whether the pot should have drainage holes
     * @return Pointer to the newly created Pot object
     */

    virtual Pot *createPot(std::string size, std::string shape, bool drainage) = 0;
};

#endif