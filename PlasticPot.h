#ifndef PLASTICPOT_H
#define PLASTICPOT_H

#include "Pot.h"

class PlasticPot: public Pot{
    public:
        PlasticPot(std::string size, std::string shape, bool drainage);
        void print() override;

};

#endif