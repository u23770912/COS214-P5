#include "MetalPotFactory.h"

Pot* MetalPotFactory::createPot(std::string size, std::string shape, bool drainage){
    return new MetalPot(size, shape, drainage);
}
