#ifndef Cashier_H
#define Cashier_H

#include "SalesFloorStaff.h"
#include "CareCommand.h"
#include <iostream>

#include "StaffMember.h"

/**
 * @class Cashier
 * @brief A Concrete Handler for sales floor tasks.
 *
 * This is an instantiable class. The chain of responsibility for the
 * sales team will be composed of instances of Cashier.
 */
class Cashier : public SalesFloorStaff
{
public:
    void handleCommand(Command *command) override
    {
        if (!isBusy())
        {
            activePlant = command->getReceiver();
            activeTask = command->getType();
            setBusyFor(std::chrono::seconds(3));
            std::cout << "Cashier is handling the '" << command->getType() << "' command." << std::endl;
            command->execute();
            delete command;
        }
        else if (next != nullptr)
        {
            std::cout << "Cashier is busy, passing to next in the sales team." << std::endl;
            next->handleCommand(command);
        }
        else
        {
            // Last handler in the chain is busy, queue the command
            if (manager)
            {
                std::cout << "All Cashiers are busy. Queueing the '" << command->getType() << "' task." << std::endl;
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

#endif // Cashier_H
