
#include "InNurseryState.h"
#include "PlantedState.h"
#include "../PlantProduct.h"
#include <iostream>

void InNurseryState::advanceState(PlantProduct* plant) {
    std::cout << "Plant is being planted. Transitioning to Planted state." << std::endl;
    plant->transitionTo(new PlantedState());
}
