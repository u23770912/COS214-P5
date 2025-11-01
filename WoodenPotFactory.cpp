#include "WoodenPotFactory.h"

Pot* WoodenPotFactory::createPot(std::string size, std::string shape, bool drainage){
    return new WoodenPot(size, shape, drainage);
}
