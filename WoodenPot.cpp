#include "WoodenPot.h"

WoodenPot::WoodenPot(std::string size, std::string shape, bool drainage) : Pot(size, shape, drainage)
{
}

void WoodenPot::print()
{
    std::cout << "Wooden Pot: [ Size: " << this->getSize() << ", Shape: " << this->getShape() << ", Drainage: " << this->getDrainage() << " ]" << std::endl;
}

std::string WoodenPot::getPotType() const
{
    return "Wooden";
}