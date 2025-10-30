#ifndef CLAYPOT_H
#define CLAYPOT_H

#include "Pot.h"

class ClayPot: public Pot{
    public:
        ClayPot(std::string size, std::string shape, bool drainage);
        void print() override;

};

#endif