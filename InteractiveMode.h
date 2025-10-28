#ifndef INTERACTIVEMODE_H
#define INTERACTIVEMODE_H

#include "ModeVisitor.h"
#include "StaffManager.h"
#include "Command.h"
#include "CareCommand.h"
#include "PlantProduct.h"
#include <iostream>

/**
 * @class InteractiveMode
 * @brief A Concrete Visitor for handling the INTERACTIVE mode.
 */
class InteractiveMode : public ModeVisitor
{
public:
    void processUpdate(StaffManager *manager, PlantProduct *plant, const std::string &commandType) override;
    void resolvePendingTask(StaffManager *manager, const std::string &userInput) override;
};

#endif // INTERACTIVEMODE_H
