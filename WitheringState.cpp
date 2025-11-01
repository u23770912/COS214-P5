#include "WitheringState.h"
#include "PlantProduct.h"
#include <iostream>

void WitheringState::onEnter(PlantProduct* plant) {
    if (plant && plant->getProfile()) {
        std::cout << "Plant " << plant->getProfile()->getSpeciesName() << " is now withering due to neglect." << std::endl;
    } else {
        std::cout << "A plant has entered the withering state due to neglect." << std::endl;
    }
}

void WitheringState::onExit(PlantProduct* plant) {
    std::cout << "Plant is exiting the withering state." << std::endl;
}

std::string WitheringState::getName() const {
    return "Withering";
}

void WitheringState::advanceState(PlantProduct* plant) {
    std::cout << "The plant remains in the withering state." << std::endl;
}