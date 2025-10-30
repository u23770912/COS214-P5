#ifndef WOODENPOTFACTORY_H
#define WOODENPOTFACTORY_H

#include "Pot.h"
#include "WoodenPot.h"
#include "PotFactory.h"

class WoodenPotFactory: public PotFactory {
    public:
        Pot* createPot(std::string size, std::string shape, bool drainage) override;
};

#endif