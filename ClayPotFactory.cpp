#include "ClayPotFactory.h"

Pot* ClayPotFactory::createPot(std::string size, std::string shape, bool drainage){
    return new ClayPot(size, shape, drainage);
}
