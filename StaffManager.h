#ifndef STAFFMANAGER_H
#define STAFFMANAGER_H

#include "LifecycleObserver.h"
#include "ModeVisitor.h" // Include the visitor
#include <string>
#include <iostream>

// Forward declarations
class StaffMember;
class PlantProduct;
class Command;

/**
 * @class StaffManager
 * @brief The "Element" class that accepts a visitor to change its mode.
 * Observes plants, creates commands, and passes them to a dispatcher.
 */
class StaffManager : public LifecycleObserver {
    private:
        StaffMember* staffDispatcher;
        ModeVisitor* currentModeVisitor; // The current mode (state)
        
        // --- Interactive Mode State ---
        PlantProduct* pendingPlant;
        std::string expectedCommandType;
    public:
        StaffManager(StaffMember* dispatcher);
        ~StaffManager();

        // --- Visitor Management ---
        void setMode(ModeVisitor* newMode);

        // --- Methods called by Visitors ---
        void setPendingTask(PlantProduct* plant, const std::string& commandType);
        void clearPendingTask();
        bool hasPendingTask() const;
        PlantProduct* getPendingPlant() const;
        std::string getExpectedCommandType() const;
        void dispatchCommand(Command* command);

        // --- Core Methods ---
        void resolvePendingTask(const std::string& userInput);
        void update(PlantProduct* plant, const std::string& commandType) override;

};

#endif // STAFFMANAGER_H