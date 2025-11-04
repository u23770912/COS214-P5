#ifndef FERTILIZE_COMMAND_H
#define FERTILIZE_COMMAND_H

#include "Command.h"
#include "PlantProduct.h"
#include <iostream>

/**
 * @brief Command class for fertilizing plants
 * 
 * This class implements the Command pattern for fertilizing operations.
 * It executes the fertilizing care action on plants.
 */
class FertilizeCommand : public Command {
public:
    // Constructor for prototype
    FertilizeCommand() : Command() {}

    void execute() override {
        if (plantReceiver) {
            plantReceiver->performCare("Fertilizing");
        }
    }

    std::string getType() const override { return "Fertilizing"; }
    std::string getRequiredRole() const override { return "Greenhouse"; }

    Command* clone() const override {
        return new FertilizeCommand(*this);
    }
};

#endif // FERTILIZE_COMMAND_H
