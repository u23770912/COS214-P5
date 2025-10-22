#ifndef REPORT_COMMAND_H
#define REPORT_COMMAND_H

#include "CareCommand.h"

class ReportCommand : public CareCommand {
public:
    ReportCommand(PlantProduct* receiver);
    
    void execute() override;
};

#endif 