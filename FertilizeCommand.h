#ifndef FERTILIZE_COMMAND_H
#define FERTILIZE_COMMAND_H

#include "CareCommand.h"

class FertilizeCommand : public CareCommand {
public:
    FertilizeCommand(PlantProduct* receiver);
    
    void execute() override;
};

#endif