#include "StaffManager.h"
#include "StaffMember.h"
#include "CareCommand.h"
#include "PlantProduct.h"

StaffManager::StaffManager(StaffMember *dispatcher)
    : staffDispatcher(dispatcher) {}

StaffManager::~StaffManager()
{
}

void StaffManager::update(PlantProduct *plant, const std::string &commandType)
{
    CareCommand *command = CareCommand::createCommand(commandType);
    if (command)
    {
        command->setReceiver(plant);
        dispatchCommand(command);
    }
}

void StaffManager::dispatchCommand(Command *command)
{
    if (staffDispatcher && command)
    {
        staffDispatcher->dispatch(command);
    }
}