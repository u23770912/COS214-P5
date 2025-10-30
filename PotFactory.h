#ifndef POTFACTORY_H
#define POTFACTORY_H

#include "Pot.h"

class PotFactory {
    public:
        virtual ~PotFactory() {}
        virtual Pot* createPot(std::string size, std::string shape, bool drainage) = 0;
};

#endif