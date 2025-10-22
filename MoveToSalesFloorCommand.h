#ifndef MOVE_TO_SALES_FLOOR_COMMAND_H
#define MOVE_TO_SALES_FLOOR_COMMAND_H

#include "CareCommand.h"

class MoveToSalesFloorCommand : public CareCommand {
public:
    MoveToSalesFloorCommand(PlantProduct* receiver);
    
    void execute() override;
};

#endif