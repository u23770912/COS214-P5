#include "PlantedState.h"
#include "GrowingState.h"
#include "../PlantProduct.h"
#include <iostream>

void PlantedState::advanceState(PlantProduct* plant) {
    std::cout << "Plant has sprouted. Transitioning to Growing state." << std::endl;
    plant->transitionTo(new GrowingState());
}