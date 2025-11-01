#include "GlassPot.h"

GlassPot::GlassPot(std::string size, std::string shape, bool drainage) : Pot(size, shape, drainage)
{
}

void GlassPot::print()
{
    std::cout << "Glass Pot: [ Size: " << this->getSize() << ", Shape: " << this->getShape() << ", Drainage: " << this->getDrainage() << " ]" << std::endl;
}

std::string GlassPot::getPotType() const
{
    return "Glass";
}