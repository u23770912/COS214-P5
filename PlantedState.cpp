#include "PlantedState.h"
#include "PlantProduct.h"
#include "ReadyForSaleState.h"  
#include <iostream>
#include <string>  

void PlantedState::onEnter(PlantProduct* plant) {
    std::cout << "Plant has entered the Planted state." << std::endl;
}

void PlantedState::onExit(PlantProduct* plant) {
    std::cout << "Plant is exiting the Planted state." << std::endl;
}

void PlantedState::advanceState(PlantProduct* plant) {
    // Assume profile returns a string, convert to int
    std::string daysStr = plant->getProfile()->getProperty("daysPlantedToReady");
    int requiredDays = daysStr.empty() ? 5 : std::stoi(daysStr);  // Default to 5 days if not set

    if (plant->  getDaysInCurrentState() >= requiredDays) {
        std::cout << "Advancing from Planted to ReadyForSale state." << std::endl;
        plant->transitionTo(new ReadyForSaleState());
    }
}

std::string PlantedState::getName() const{
    return "Planted";
}

