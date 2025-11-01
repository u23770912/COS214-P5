#include "PlasticPotFactory.h"

Pot* PlasticPotFactory::createPot(std::string size, std::string shape, bool drainage){
    return new PlasticPot(size, shape, drainage);
}
