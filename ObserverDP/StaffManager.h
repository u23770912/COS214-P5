#ifndef STAFFMANAGER_H
#define STAFFMANAGER_H

#include "LifeCycleObserver.h"  // Now includes the factory logic
#include "../ChainOfRespDP/StaffMember.h"
#include <string>

/**
 * @class StaffManager
 * @brief Observes plants, creates commands, and passes them to a dispatcher.
 * 
 * This class uses the static factory methods on CareCommand to create commands.
 */
class StaffManager : public LifecycleObserver {
private:
    StaffMember* staffDispatcher; // A pointer to the single dispatcher object

public:
    StaffManager(StaffMember* dispatcher) 
        : staffDispatcher(dispatcher) {}

    /**
     * @brief The update method called by the Observable (PlantProduct).
     * Creates a command and passes it to the staff dispatcher.
     */
    void update(PlantProduct* plant, const std::string& commandType) override {
        // Use the static factory method on Command
        Command* command = Command::createCommand(commandType);
        if (command) {
            command->setReceiver(plant);
            staffDispatcher->dispatch(static_cast<Command*>(command));
        }
    }
    


};

#endif // STAFFMANAGER_H