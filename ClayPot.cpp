#include "ClayPot.h"

ClayPot::ClayPot(std::string size, std::string shape, bool drainage) : Pot(size, shape, drainage)
{
}

void ClayPot::print()
{
    std::cout << "Clay Pot: [ Size: " << this->getSize() << ", Shape: " << this->getShape() << ", Drainage: " << this->getDrainage() << " ]" << std::endl;
}

std::string ClayPot::getPotType() const
{
    return "Clay";
}