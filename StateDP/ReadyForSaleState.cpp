#include "ReadyForSaleState.h"
#include "../PlantProduct.h"
#include "../LifeCycleMonitor.h"
#include <iostream>

void ReadyForSaleState::onEnter(PlantProduct* plant) {
    std::cout << "Plant entered ReadyForSale state - notifying observers to move to sales floor." << std::endl;
    
    // Notify the monitor that the plant is ready for sale
    // This will trigger the StaffManager to create a MoveToSalesFloorCommand
    plant->notify("MoveToSalesFloor");
}

void ReadyForSaleState::onExit(PlantProduct* plant) {
    std::cout << "Plant exiting ReadyForSale state." << std::endl;
}

void ReadyForSaleState::advanceState(PlantProduct* plant) {
    // ReadyForSale is typically a stable state - no automatic advancement
    std::cout << "Plant is ready for sale - no further lifecycle advancement." << std::endl;
}
