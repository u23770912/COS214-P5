#include "SalesAssociate.h"
#include <iostream>

SalesAssociate::SalesAssociate(std::string name) : StaffMember(name, "Sales Associate") {}

void SalesAssociate::handleCommand(Command* command) {
    if (canHandle(command)) {
        std::cout << "Sales Associate " << name << " is handling the command." << std::endl;
        command->execute();
    } else {
        StaffMember::handleCommand(command);
    }
}
