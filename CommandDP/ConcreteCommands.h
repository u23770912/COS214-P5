#ifndef CONCRETE_COMMANDS_H
#define CONCRETE_COMMANDS_H

#include "CareCommand.h"
#include "../PlantProduct.h"
#include <iostream>

// --- Concrete Commands ---

class WaterCommand : public CareCommand {
public:
    void execute() override {
        std::cout << "Executing Water Command..." << std::endl;
        plantReceiver->waterYourself();
    }
    CareCommand* clone() const override { return new WaterCommand(*this); }
};

class PruneCommand : public CareCommand {
public:
    void execute() override {
        std::cout << "Executing Prune Command on plant." << std::endl;
        // In a real system: plantReceiver->prune();
    }
    CareCommand* clone() const override { return new PruneCommand(*this); }
};

class FertilizeCommand : public CareCommand {
public:
    void execute() override {
        std::cout << "Executing Fertilize Command on plant." << std::endl;
        // In a real system: plantReceiver->fertilize();
    }
    CareCommand* clone() const override { return new FertilizeCommand(*this); }
};

class MoveToSalesFloorCommand : public CareCommand {
public:
    void execute() override {
        std::cout << "Executing Move To Sales Floor Command." << std::endl;
        // In a real system: plantReceiver->setLocation("Sales Floor");
    }
    CareCommand* clone() const override { return new MoveToSalesFloorCommand(*this); }
};

#endif // CONCRETE_COMMANDS_H
