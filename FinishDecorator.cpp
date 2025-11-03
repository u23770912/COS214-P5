#include "FinishDecorator.h"
#include <iostream>

FinishDecorator::FinishDecorator(Pot* pot, const std::string& finish) 
    : PotDecorator(pot, getFinishPrice(finish)), 
      finish(finish) {
}

void FinishDecorator::print() {
    wrappedPot->print();
    std::cout << " + " << finish << " Finish (R" << decorationPrice << ")";
}

double FinishDecorator::getFinishPrice(const std::string& finish) const {
    if (finish == "Glazed" || finish == "Weathered") return 4.0;
    if (finish == "Textured") return 3.0;
    return 2.0;
}