#ifndef AUTONOMOUSMODE_H
#define AUTONOMOUSMODE_H

#include "ModeVisitor.h"
#include "StaffManager.h"
#include "Command.h"
#include "CareCommand.h"
#include "PlantProduct.h"
#include <iostream>

/**
 * @class AutonomousMode
 * @brief A Concrete Visitor for handling the AUTONOMOUS mode.
 */
class AutonomousMode : public ModeVisitor {
public:
    void processUpdate(StaffManager* manager, PlantProduct* plant, const std::string& commandType) override;
};

#endif // AUTONOMOUSMODE_H