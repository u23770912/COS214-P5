#ifndef METALPOTFACTORY_H
#define METALPOTFACTORY_H

#include "Pot.h"
#include "MetalPot.h"
#include "PotFactory.h"

class MetalPotFactory: public PotFactory {
    public:
        Pot* createPot(std::string size, std::string shape, bool drainage) override;
};

#endif