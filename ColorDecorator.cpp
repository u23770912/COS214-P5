#include "ColorDecorator.h"
#include <iostream>

ColorDecorator::ColorDecorator(Pot* pot, const std::string& color) 
    : PotDecorator(pot, isMetallic(color) ? 5.0 : 3.0), 
      color(color) {
}

void ColorDecorator::print() {
    wrappedPot->print();
    std::cout << " + " << color << " Color (R" << decorationPrice << ")";
}

bool ColorDecorator::isMetallic(const std::string& color) const {
    return color.find("Gold") != std::string::npos ||
           color.find("Silver") != std::string::npos ||
           color.find("Copper") != std::string::npos ||
           color.find("Bronze") != std::string::npos;
}