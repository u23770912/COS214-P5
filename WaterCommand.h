#ifndef WATER_COMMAND_H
#define WATER_COMMAND_H

#include "Command.h"
#include "PlantProduct.h"
#include <iostream>

/**
 * @brief Command class for watering plants
 * 
 * This class implements the Command pattern for watering operations.
 * It executes the watering care action on plants in the greenhouse.
 */
class WaterCommand : public Command {
public:
    WaterCommand() : Command() {}
    void execute() override {
        std::cout << "Executing Water Command..." << std::endl;
        if (plantReceiver) {
            plantReceiver->performCare("Watering");
        }
    }

    std::string getType() const override { return "Watering"; }

    std::string getRequiredRole() const override { return "Greenhouse"; }

    Command* clone() const override {
        return new WaterCommand(*this);
    }
};

#endif // WATER_COMMAND_H