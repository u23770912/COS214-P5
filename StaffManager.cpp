#include "StaffManager.h"
#include "StaffMember.h"
#include "AutonomousMode.h"

StaffManager::StaffManager(StaffMember* dispatcher) 
    : staffDispatcher(dispatcher), currentModeVisitor(new AutonomousMode()) {}

StaffManager::~StaffManager() {
    delete currentModeVisitor;
}

void StaffManager::setMode(ModeVisitor* newMode) {
    delete currentModeVisitor;
    currentModeVisitor = newMode;
}

void StaffManager::update(PlantProduct* plant, const std::string& commandType) {
    currentModeVisitor->processUpdate(this, plant, commandType);
}

void StaffManager::dispatchCommand(Command* command) {
    if (staffDispatcher && command) {
        staffDispatcher->dispatch(command);
    }
}