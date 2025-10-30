//should be romoved
#include "GrowingState.h"
#include "ReadyForSaleState.h"
#include "../PlantProduct.h"
#include <iostream>

void GrowingState::onEnter(PlantProduct* plant) {
    std::cout << "Plant entered Growing state - actively developing." << std::endl;
}

void GrowingState::onExit(PlantProduct* plant) {
    std::cout << "Plant exiting Growing state." << std::endl;
}

void GrowingState::advanceState(PlantProduct* plant) {
    std::cout << "Plant has matured sufficiently. Transitioning to ReadyForSale state." << std::endl;
    plant->transitionTo(new ReadyForSaleState());
}
