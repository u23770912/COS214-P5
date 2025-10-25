#ifndef SALESASSOCIATE_H
#define SALESASSOCIATE_H

#include "SalesFloorStaff.h"
#include <iostream>

/**
 * @class SalesAssociate
 * @brief A Concrete Handler for sales floor tasks.
 * 
 * This is an instantiable class. The chain of responsibility for the
 * sales team will be composed of instances of SalesAssociate.
 */
class SalesAssociate : public SalesFloorStaff {
public:
    void handleCommand(Command* command) override {
        if (!isBusy()) {
            std::cout << "Sales Associate is handling the '" << command->getType() << "' command." << std::endl;
            setBusy(true);
            command->execute();
            setBusy(false);
            delete command;
        } else if (next != nullptr) {
            std::cout << "Sales Associate is busy, passing to next in the sales team." << std::endl;
            next->handleCommand(command);
        } else {
            std::cout << "All Sales Associates are busy. The '" << command->getType() << "' task was dropped." << std::endl;
            // TODO: Plant will wither - this functionality will be added when CareCommand is implemented
            // command->getReceiver()->transitionToWithering();
            delete command;
        }
    }
};

#endif // SALESASSOCIATE_H