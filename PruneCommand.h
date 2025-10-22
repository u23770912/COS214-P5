#ifndef PRUNE_COMMAND_H
#define PRUNE_COMMAND_H

#include "CareCommand.h"

class PruneCommand : public CareCommand {
public:
    PruneCommand(PlantProduct* receiver);
    
    void execute() override;
};

#endif