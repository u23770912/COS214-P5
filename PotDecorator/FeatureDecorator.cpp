#include "FeatureDecorator.h"
#include <iostream>

FeatureDecorator::FeatureDecorator(Pot* pot, const std::string& feature) 
    : PotDecorator(pot, getFeaturePrice(feature)), 
      feature(feature) {
}

void FeatureDecorator::print() {
    wrappedPot->print();
    std::cout << " + " << feature << " Feature (R" << decorationPrice << ")";
}

double FeatureDecorator::getFeaturePrice(const std::string& feature) const {
    if (feature == "Self-Watering System") return 25.0;
    if (feature == "LED Lighting") return 20.0;
    if (feature == "Wheeled Base") return 15.0;
    if (feature == "Insulated Walls") return 12.0;
    if (feature == "UV Protection") return 10.0;
    if (feature == "Drainage Tray") return 8.0;
    return 5.0;
}