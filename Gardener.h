#ifndef GARDENER_H
#define GARDENER_H

#include "GreenhouseStaff.h"
#include "Command.h"
#include <iostream>

#include "StaffMember.h"

/**
 * @class Gardener
 * @brief A Concrete Handler for greenhouse tasks.
 *
 * This is an instantiable class. The chain of responsibility for the
 * greenhouse team will be composed of instances of Gardener.
 */
class Gardener : public GreenhouseStaff
{

public:
    void handleCommand(Command *command) override
    {
        if (!isBusy())
        {
            activePlant = command->getReceiver();
            activeTask = command->getType();
            setBusyFor(std::chrono::seconds(3));
            std::cout << "Gardener is handling the '" << command->getType() << "' command." << std::endl;
            command->execute();
            delete command;
        }
        else if (next != nullptr)
        {
            std::cout << "Gardener is busy, passing to next in the greenhouse team." << std::endl;
            next->handleCommand(command);
        }
        else
        {
            // Last handler in the chain is busy, queue the command
            if (manager)
            {
                std::cout << "All Gardeners are busy. Queueing the '" << command->getType() << "' task." << std::endl;
                manager->queueUnhandledCommand(command);
            }
            else
            {
                // Fallback in case manager is not set
                std::cout << "Error: Manager not set. The '" << command->getType() << "' task was dropped." << std::endl;
                delete command;
            }
        }
    }
};

#endif // GARDENER_H
