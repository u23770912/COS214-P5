#include "GlassPotFactory.h"

Pot* GlassPotFactory::createPot(std::string size, std::string shape, bool drainage){
    return new GlassPot(size, shape, drainage);
}
