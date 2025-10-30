#ifndef METALPOT_H
#define METALPOT_H

#include "Pot.h"

class MetalPot: public Pot{
    public:
        MetalPot(std::string size, std::string shape, bool drainage);
        void print() override;

};


#endif
