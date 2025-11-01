#include "PatternDecorator.h"
#include <iostream>

PatternDecorator::PatternDecorator(Pot* pot, const std::string& pattern) 
    : PotDecorator(pot, getPatternPrice(pattern)), 
      pattern(pattern) {
}

void PatternDecorator::print() {
    wrappedPot->print();
    std::cout << " + " << pattern << " Pattern (R" << decorationPrice << ")";
}

double PatternDecorator::getPatternPrice(const std::string& pattern) const {
    if (pattern.find("Hand-Painted") != std::string::npos) return 8.0;
    if (pattern.find("Floral") != std::string::npos || 
        pattern.find("Botanical") != std::string::npos) return 6.0;
    return 4.0;
}