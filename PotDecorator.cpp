#include "PotDecorator.h"
#include "Pot.h"
#include <string>

// Implementation of PotDecorator

PotDecorator::PotDecorator(Pot* pot, double price) 
    : Pot("", "", false), wrappedPot(pot), decorationPrice(price) {
    // Base Pot constructor called with default values since decorator delegates to wrappedPot
}

PotDecorator::~PotDecorator() {
    delete wrappedPot;
}

double PotDecorator::getPrice() const {
    // Check if wrapped pot is also a decorator
    const PotDecorator* decorator = dynamic_cast<const PotDecorator*>(wrappedPot);
    if (decorator) {
        // Recursively get price from decorator chain
        return decorationPrice + decorator->getPrice();
    } else {
        // Base pot price (R10.00) + this decoration
        return decorationPrice + 10.0;
    }
}

std::string PotDecorator::getPotType() const {
    if (wrappedPot) {
        return wrappedPot->getPotType();
    }
    return "Unknown";
}