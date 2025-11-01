#include "Pot.h"

Pot::Pot(std::string size, std::string shape, bool drainage) : size(size), shape(shape), drainage(drainage)
{
}

Pot::~Pot()
{
}

void Pot::print()
{
}

std::string Pot::getShape()
{
    return this->shape;
}

std::string Pot::getSize()
{
    return this->size;
}

std::string Pot::getDrainage()
{
    return this->drainage ? "Yes" : "No";
}
