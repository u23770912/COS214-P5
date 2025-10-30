#ifndef CLAYPOTFACTORY_H
#define CLAYPOTFACTORY_H

#include "Pot.h"
#include "ClayPot.h"
#include "PotFactory.h"

class ClayPotFactory: public PotFactory {
    public:
        Pot* createPot(std::string size, std::string shape, bool drainage) override;
};

#endif