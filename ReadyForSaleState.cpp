#include "ReadyForSaleState.h"
#include "PlantProduct.h"
#include <iostream>

void ReadyForSaleState::onEnter(PlantProduct *plant)
{
    std::cout << "[STATE] Plant has entered ReadyForSale state (terminal state)" << std::endl;
    hasRequestedMove = false;
}

void ReadyForSaleState::onExit(PlantProduct *plant)
{
    std::cout << "[STATE] Plant is exiting ReadyForSale state" << std::endl;
}

void ReadyForSaleState::advanceState(PlantProduct *plant)
{
    // Only request move once, after a delay
    if (!hasRequestedMove)
    {
        int secondsInState = plant->getSecondsInCurrentState();

        // Wait 5 seconds before requesting move (give time to see the state change)
        if (secondsInState >= 5)
        {
            std::cout << "[READY_FOR_SALE] Requesting move to sales floor..." << std::endl;
            plant->notify("MoveToSalesFloor");
            hasRequestedMove = true;
        }
    }

    // Terminal state - stays here, won't wither
    // Plant remains in this state until explicitly sold or removed
}