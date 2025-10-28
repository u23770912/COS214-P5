#ifndef Cashier_H
#define Cashier_H

#include "SalesFloorStaff.h"
#include "CareCommand.h"
#include <iostream>

/**
 * @class Cashier
 * @brief A Concrete Handler for sales floor tasks.
 * 
 * This is an instantiable class. The chain of responsibility for the
 * sales team will be composed of instances of Cashier.
 */
class Cashier : public SalesFloorStaff {
public:
    void handleCommand(Command* command) override {
        if (!isBusy()) {
<<<<<<< HEAD
            std::cout << "Cashier is handling the '" << command->getType() << "' command." << std::endl;
            setBusy(true);
            command->execute();
            setBusy(false);
=======
            activePlant = command->getReceiver();
            setBusyFor(std::chrono::seconds(20));  // stay busy twenty seconds
            std::cout << "Cashier is handling the '" << command->getType() << "' command." << std::endl;
            command->execute();
>>>>>>> a5cc157f06bf6b3c50e503d2c0883ab9968a4990
            delete command;
        } else if (next != nullptr) {
            std::cout << "Cashier is busy, passing to next in the sales team." << std::endl;
            next->handleCommand(command);
        } else {
            std::cout << "All Cashiers are busy. The '" << command->getType() << "' task was dropped. Plant will wither." << std::endl;
              // Only transition to withering if there is a plant associated with the command.
           if(CareCommand* careCmd = dynamic_cast<CareCommand*>(command)) {
               if (careCmd->getReceiver()) {
                   careCmd->getReceiver()->transitionToWithering();
               }
            }
            delete command;
        }
    }
};

#endif // Cashier_H
