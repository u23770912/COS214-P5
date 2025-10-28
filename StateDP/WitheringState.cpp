#include "WitheringState.h"
#include "../PlantProduct.h"
#include <iostream>

void WitheringState::onEnter(PlantProduct* plant) {
    if (plant && plant->getProfile()) {
        std::cout << "Plant " << plant->getProfile()->getSpeciesName() 
                  << " is now withering due to neglect." << std::endl;
    } else {
        std::cout << "A plant has entered the withering state due to neglect." << std::endl;
    }
}

void WitheringState::onExit(PlantProduct* plant) {
    // Typically no action needed when exiting a terminal state
    std::cout << "Plant is exiting the withering state." << std::endl;
}

void WitheringState::advanceState(PlantProduct* plant) {
    // WitheringState might not advance further
    std::cout << "Plant remains in withering state." << std::endl;
}

std::string WitheringState::getName() const {
    return "Withering";
}