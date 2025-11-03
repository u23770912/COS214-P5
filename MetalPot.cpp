#include "MetalPot.h"

MetalPot::MetalPot(std::string size, std::string shape, bool drainage) : Pot(size, shape, drainage)
{
}

void MetalPot::print()
{
        std::cout << "Metal Pot: [ Size: " << this->getSize() << ", Shape: " << this->getShape() << ", Drainage: " << this->getDrainage() << " ]" << std::endl;
}

std::string MetalPot::getPotType() const
{
        return "Metal";
}