#ifndef INTERACTIVEMODE_H
#define INTERACTIVEMODE_H

#include "ModeVisitor.h"
#include "StaffManager.h"
#include "Command.h"
#include "CareCommand.h"
#include "PlantProduct.h"
#include <iostream>
#include <string>

/**
 * @class InteractiveMode
 * @brief A Concrete Visitor for handling the INTERACTIVE mode.
 */
class InteractiveMode : public ModeVisitor {
private:
    PlantProduct* pendingPlant;
    std::string expectedCommand;
    
public:
    InteractiveMode() : pendingPlant(nullptr), expectedCommand("") {}
    
    void processUpdate(StaffManager* manager, PlantProduct* plant, const std::string& commandType) override;
};

#endif // INTERACTIVEMODE_H