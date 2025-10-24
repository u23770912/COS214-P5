#ifndef READY_FOR_SALE_STATE_H
#define READY_FOR_SALE_STATE_H

#include "PlantState.h"
#include <string>

class ReadyForSaleState : public PlantState {
public:
    void onEnter(PlantProduct* plant) override {}
    void onExit(PlantProduct* plant) override {}
    std::string getName() const override { return "ReadyForSale"; }
};

#endif // READY_FOR_SALE_STATE_H
