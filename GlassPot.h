#ifndef GLASSPOT_H
#define GLASSPOT_H

#include "Pot.h"

class GlassPot: public Pot{
    public:
        GlassPot(std::string size, std::string shape, bool drainage);
        void print() override;

};

#endif