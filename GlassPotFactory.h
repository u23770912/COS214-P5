#ifndef GLASSPOTFACTORY_H
#define GLASSPOTFACTORY_H

#include "Pot.h"
#include "GlassPot.h"
#include "PotFactory.h"

class GlassPotFactory : public PotFactory {
    public:
        Pot* createPot(std::string size, std::string shape, bool drainage) override;
};

#endif