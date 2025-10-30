#ifndef WOODENPOT_H
#define WOODENPOT_H

#include "Pot.h"

class WoodenPot: public Pot{
    public:
        WoodenPot(std::string size, std::string shape, bool drainage);
        void print() override;

};

#endif