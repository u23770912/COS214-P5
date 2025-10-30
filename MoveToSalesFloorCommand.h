#ifndef MOVE_TO_SALES_FLOOR_COMMAND_H
#define MOVE_TO_SALES_FLOOR_COMMAND_H

#include "CareCommand.h"
#include <iostream>

class MoveToSalesFloorCommand : public CareCommand {
public:
    // Constructor for prototype
    MoveToSalesFloorCommand() : CareCommand() {}
    void execute() override;

    std::string getType() const override { return "MoveToSalesFloor"; }
    std::string getRequiredRole() const override { return "Sales"; }

    CareCommand* clone() const override {
        return new MoveToSalesFloorCommand(*this);
    }
};

#endif // MOVE_TO_SALES_FLOOR_COMMAND_H
