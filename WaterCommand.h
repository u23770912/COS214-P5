#ifndef WATER_COMMAND_H
#define WATER_COMMAND_H

#include "CareCommand.h"

class WaterCommand : public CareCommand {
public:
    WaterCommand(PlantProduct* receiver);
    
    void execute() override;
};

#endif 