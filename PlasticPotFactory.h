#ifndef PLASTICPOTFACTORY_H
#define PLASTICPOTFACTORY_H

#include "Pot.h"
#include "PlasticPot.h"
#include "PotFactory.h"


class PlasticPotFactory: public PotFactory {
    public:
        Pot* createPot(std::string size, std::string shape, bool drainage) override;
};

#endif