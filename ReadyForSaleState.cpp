#include "ReadyForSaleState.h"
#include "PlantProduct.h"
#include "WitheringState.h"  
#include <iostream>
#include <string>  

void ReadyForSaleState::onEnter(PlantProduct* plant) {
    std::cout << "Plant has entered the ReadyForSale state." << std::endl;
}

void ReadyForSaleState::onExit(PlantProduct* plant) {
    std::cout << "Plant is exiting the ReadyForSale state." << std::endl;
}

void ReadyForSaleState::advanceState(PlantProduct* plant) {
    // Check for neglect: transition to WitheringState if no care or sale within max days
    std::string maxDaysStr = plant->getProfile()->getProperty("maxDaysWithoutCare");
    int maxDays = maxDaysStr.empty() ? 7 : std::stoi(maxDaysStr);  // Default to 7 days if not set

    if (plant->getDaysInCurrentState() >= maxDays) {
        std::cout << "Plant neglected too long in ReadyForSale state, transitioning to Withering." << std::endl;
        plant->transitionTo(new WitheringState());
    } else {
        std::cout << "Plant remains in ReadyForSale state." << std::endl;
    }
    // Note: daysInCurrentState is incremented in PlantProduct::advanceLifecycle()
}