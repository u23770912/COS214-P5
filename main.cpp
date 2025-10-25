#include <iostream>
#include <vector>

// Core Components
#include "PlantProduct.h"

// Bridge Pattern
#include "BridgeDP/PlantSpeciesProfile.h"
#include "BridgeDP/FlowerProfile.h"
#include "BridgeDP/TreeProfile.h"
#include "BridgeDP/SucculentProfile.h"

// State Pattern
#include "StateDP/PlantState.h"
#include "StateDP/PlantedState.h"
#include "StateDP/InNurseryState.h"
#include "StateDP/ReadyForSaleState.h"

// Observer + Command + CoR
#include "ObserverDP/StaffManager.h"
#include "VisitorDP/AutonomousMode.h"
#include "VisitorDP/InteractiveMode.h"
// CoR
#include "ChainOfRespDP/StaffMember.h"
#include "ChainOfRespDP/Gardener.h"
#include "ChainOfRespDP/Cashier.h"

// Concrete Commands
#include "CommandDP/Command.h"
#include "CommandDP/WaterCommand.h"
#include "CommandDP/PruneCommand.h"
#include "CommandDP/FertilizeCommand.h"
#include "CommandDP/MoveToSalesFloorCommand.h"

// Strategy Pattern
#include "StrategyDP/CareStrategy.h"
#include "StrategyDP/DripWateringStrategy.h"
#include "StrategyDP/GentleMistStrategy.h"
#include "StrategyDP/FertilizingStrategy.h"
#include "StrategyDP/StandardPruningStrategy.h"

// A simple concrete state to trigger a notification
class NeedsWaterState : public PlantState {
public:
    void onEnter(PlantProduct* plant) override {
        std::cout << "Plant " << "plant->getPlantId()" << " has entered NeedsWaterState. Notifying StaffManager." << std::endl;
        plant->notify("Watering"); // Notify that a "Watering" command is needed
    }
    void onExit(PlantProduct* plant) override {}
    std::string getName() const override { return "NeedsWater"; }
    void advanceState(PlantProduct* plant) override {
        // After watering, maybe it goes back to a general "InNursery" state
        //plant->transitionTo(new InNurseryState());
    }
};

// A new state to trigger fertilization
class NeedsFertilizerState : public PlantState {
public:
    void onEnter(PlantProduct* plant) override {
        std::cout << "Plant " << "plant->getPlantId()" << " has entered NeedsFertilizerState. Notifying StaffManager." << std::endl;
        plant->notify("Fertilizing"); // Notify that a "Fertilizing" command is needed
    }
    void onExit(PlantProduct* plant) override {}
    std::string getName() const override { return "NeedsFertilizer"; }
    void advanceState(PlantProduct* plant) override {
        //plant->transitionTo(new InNurseryState());
    }
};

class PlantedState : public PlantState {
public:
    void onEnter(PlantProduct* plant) override {
        std::cout << "Plant " << "plant->getPlantId()" << " has entered PlantedState. Notifying StaffManager." << std::endl;
        plant->notify("Planted"); // Notify that a "Planted" command is needed
    }
    void onExit(PlantProduct* plant) override {}
    std::string getName() const override { return "Planted"; }
    void advanceState(PlantProduct* plant) override {
        //plant->transitionTo(new InNurseryState());
    }
};


void setupCommandPrototypes() {
    //Command::registerCommand("Watering", new WaterCommand());
    Command::registerCommand("Pruning", new PruneCommand());
    Command::registerCommand("Fertilizing", new FertilizeCommand());
    Command::registerCommand("MoveToSalesFloor", new MoveToSalesFloorCommand());
}

int main() {
    std::cout << "--- Setting up Nursery Infrastructure ---" << std::endl;

    // 1. Setup Command Prototypes
    setupCommandPrototypes();
    std::cout << "Command prototypes registered." << std::endl;

    // 2. Build Staff and Chains
    StaffMember* dispatcher = new StaffMember();
    
    Gardener* gardener1 = new Gardener();
    gardener1->setNext(new Gardener());
    dispatcher->registerTeam("Gardener", gardener1);

    Cashier* sales1 = new Cashier();
    sales1->setNext(new Cashier());
    dispatcher->registerTeam("Cashier", sales1);

    std::cout << "Staff teams and chains created." << std::endl;

    // 3. Setup the Manager (Observer)
    StaffManager* manager = new StaffManager(dispatcher);
    std::cout << "StaffManager (Manager) created." << std::endl;

    // --- GAME START ---
    std::cout << "\n--- Welcome to the Interactive Nursery! ---" << std::endl;
    manager->setMode(new InteractiveMode());

    // 4. Create a Plant (Rose)
    PlantSpeciesProfile* roseProfile = new FlowerProfile("Rose", "A beautiful red rose", "Full sun", "well-drained soil");
    PlantProduct rose("rose_01", roseProfile);
    rose.setObserver(manager); 

    rose.addStrategy("Watering", new DripWateringStrategy());
    rose.addStrategy("Pruning", new StandardPruningStrategy());
    
    std::cout << "\n--- A new Rose has been planted. ---" << std::endl;
    rose.transitionTo(new PlantedState());
    std::cout << "Current state: " << rose.getCurrentStateName() << std::endl;

    // --- SCENARIO 1: Correct Action ---
    std::cout << "\n--- Time passes. The rose looks thirsty. ---" << std::endl;
    rose.transitionTo(new NeedsWaterState()); // Triggers manager.update()

    if (manager->hasPendingTask()) {
        std::cout << "\nAn action is required for plant 'rose_01'!" << std::endl;
        std::cout << "What should be done? (Enter command type, e.g., Watering, Pruning, skip): ";
        std::string userInput;
        std::cin >> userInput;
        manager->resolvePendingTask(userInput);
    }
    
    // Check plant state after action
    std::cout << "Plant 'rose_01' current state: " << rose.getCurrentStateName() << std::endl;


    // --- SCENARIO 2: Incorrect Action ---
    std::cout << "\n--- More time passes. The rose needs fertilizing, but we'll make a mistake. ---" << std::endl;
    rose.transitionTo(new NeedsFertilizerState()); // Triggers manager.update()

    if (manager->hasPendingTask()) {
        std::cout << "\nAn action is required for plant 'rose_01'!" << std::endl;
        std::cout << "What should be done? (Enter command type, e.g., Watering, Pruning, Fertilizing): ";
        std::string userInput;
        std::cin >> userInput;
        manager->resolvePendingTask(userInput);
    }

    // Check plant state after incorrect action
    std::cout << "Plant 'rose_01' current state: " << rose.getCurrentStateName() << std::endl;


    std::cout << "\n--- Cleaning up. ---" << std::endl;
    // Proper memory management for the chain is complex.
    // For this demo, we are simplifying cleanup.
    delete dispatcher;
    delete manager;
    delete gardener1; // Deleting the head of the chain
    delete sales1;
    delete roseProfile;

    return 0;
}
