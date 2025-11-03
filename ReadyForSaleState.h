#ifndef READY_FOR_SALE_STATE_H
#define READY_FOR_SALE_STATE_H

#include "PlantState.h"
#include <string>

class ReadyForSaleState : public PlantState {
private:
    bool hasRequestedMove;
    
public:
    ReadyForSaleState() : hasRequestedMove(false) {}
    
    void onEnter(PlantProduct* plant) override;
    void onExit(PlantProduct* plant) override;
    void advanceState(PlantProduct* plant) override;
    std::string getName() const override { return "ReadyForSale"; }
};

#endif // READY_FOR_SALE_STATE_H
