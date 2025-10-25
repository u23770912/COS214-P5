#include "StaffManager.h"
#include "../ChainOfRespDP/StaffMember.h"
#include "VisitorDP/AutonomousMode.h" // Default mode

StaffManager::StaffManager(StaffMember* dispatcher) 
    : staffDispatcher(dispatcher), currentModeVisitor(new AutonomousMode()), pendingPlant(nullptr) {}

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

void StaffManager::resolvePendingTask(const std::string& userInput) {
    currentModeVisitor->resolvePendingTask(this, userInput);
}

void StaffManager::dispatchCommand(Command* command) {
    if (staffDispatcher && command) {
        staffDispatcher->dispatch(command);
    }
}

// --- Getters and Setters for Visitors ---

void StaffManager::setPendingTask(PlantProduct* plant, const std::string& commandType) {
    pendingPlant = plant;
    expectedCommandType = commandType;
}

void StaffManager::clearPendingTask() {
    pendingPlant = nullptr;
    expectedCommandType = "";
}

bool StaffManager::hasPendingTask() const {
    return pendingPlant != nullptr;
}

PlantProduct* StaffManager::getPendingPlant() const {
    return pendingPlant;
}

std::string StaffManager::getExpectedCommandType() const {
    return expectedCommandType;
}
