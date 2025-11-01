#ifndef PRUNE_COMMAND_H
#define PRUNE_COMMAND_H

#include "CareCommand.h"
#include "PlantProduct.h"
#include <iostream>

class PruneCommand : public CareCommand {
public:
    PruneCommand() : CareCommand() {}
    void execute() override {
        std::cout << "Executing Prune Command..." << std::endl;
        if (plantReceiver) {
            plantReceiver->performCare("Pruning");
        }
    }

    std::string getType() const override { return "Pruning"; }

    std::string getRequiredRole() const override { return "Greenhouse"; }
    CareCommand* clone() const override { return new PruneCommand(*this); }
};

#endif // PRUNE_COMMAND_H
