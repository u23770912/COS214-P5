#include "WitheringState.h"
#include "../PlantProduct.h"
#include <iostream>

// The logic for WitheringState is already in the header file.
// This .cpp file is created for consistency and for any future, more complex logic.

WitheringState::WitheringState() {}
WitheringState::~WitheringState() {}

void WitheringState::onEnter(PlantProduct* product) {
    std::cout << "Plant has entered the withering state." << std::endl;
}

void WitheringState::onExit(PlantProduct* product) {
    std::cout << "Plant is exiting the withering state." << std::endl;
}

void WitheringState::advanceState(PlantProduct* product) {
    // WitheringState might not advance further, or you can define logic here
    std::cout << "Plant remains in withering state." << std::endl;
}

std::string WitheringState::getName() const {
    return "Withering";
}