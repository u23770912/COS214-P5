#include "InNurseryState.h"
#include "PlantProduct.h"
#include "PlantedState.h"  
#include <iostream>
#include <string>  

void InNurseryState::onEnter(PlantProduct* plant) {
    std::cout << "Plant has entered the InNursery state." << std::endl;
}

void InNurseryState::onExit(PlantProduct* plant) {
    std::cout << "Plant is exiting the InNursery state." << std::endl;
}

void InNurseryState::advanceState(PlantProduct* plant) {
    // Assume profile returns a string, convert to int
    std::string daysStr = plant->getProfile()->getProperty("daysInNursery");
    int requiredDays = daysStr.empty() ? 3 : std::stoi(daysStr);  // Default to 3 days if not set

    if (plant-> getDaysInCurrentState()  >= requiredDays) {
        std::cout << "Advancing from InNursery to Planted state." << std::endl;
        plant->transitionTo(new PlantedState());
    }
}