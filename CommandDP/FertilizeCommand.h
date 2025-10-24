#ifndef FERTILIZE_COMMAND_H
#define FERTILIZE_COMMAND_H

#include "CareCommand.h"
#include "../PlantProduct.h"
#include <iostream>

class FertilizeCommand : public CareCommand {
public:
    // Constructor for prototype
    FertilizeCommand() : CareCommand() {}

    void execute() override {
        if (plantReceiver) {
            plantReceiver->performCare("Fertilizing");
        }
    }

    std::string getType() const override { return "Fertilizing"; }
    std::string getRequiredRole() const override { return "Greenhouse"; }

    CareCommand* clone() const override {
        return new FertilizeCommand(*this);
    }
};

#endif // FERTILIZE_COMMAND_H
