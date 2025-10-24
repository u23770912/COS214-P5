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
// CoR
#include "ChainOfRespDP/StaffMember.h"
#include "ChainOfRespDP/Gardener.h"
#include "ChainOfRespDP/SalesAssociate.h"

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
        std::cout << "Plant " << plant->getPlantId() << " has entered NeedsWaterState. Notifying StaffManager." << std::endl;
        plant->notify("Watering"); // Notify that a "Watering" command is needed
    }
    void onExit(PlantProduct* plant) override {}
    std::string getName() const override { return "NeedsWater"; }
    void advanceState(PlantProduct* plant) override {
        // After watering, maybe it goes back to a general "InNursery" state
        plant->transitionTo(new InNurseryState());
    }
};

// A new state to trigger fertilization
class NeedsFertilizerState : public PlantState {
public:
    void onEnter(PlantProduct* plant) override {
        std::cout << "Plant " << plant->getPlantId() << " has entered NeedsFertilizerState. Notifying StaffManager." << std::endl;
        plant->notify("Fertilizing"); // Notify that a "Fertilizing" command is needed
    }
    void onExit(PlantProduct* plant) override {}
    std::string getName() const override { return "NeedsFertilizer"; }
    void advanceState(PlantProduct* plant) override {
        plant->transitionTo(new InNurseryState());
    }
};


void setupCommandPrototypes() {
    Command::registerCommand("Watering", new WaterCommand());
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
    
    // Greenhouse Team (Expanded)
    Gardener* gardener1 = new Gardener("Alice");
    Gardener* gardener2 = new Gardener("Bob");
    Gardener* gardener3 = new Gardener("Charlie");
    Gardener* gardener4 = new Gardener("David");
    gardener1->setNext(gardener2);
    gardener2->setNext(gardener3);
    gardener3->setNext(gardener4);
    dispatcher->registerTeam("Gardener", gardener1);

    // Sales Team (Expanded)
    SalesAssociate* sales1 = new SalesAssociate("Eve");
    SalesAssociate* sales2 = new SalesAssociate("Frank");
    sales1->setNext(sales2);
    dispatcher->registerTeam("Sales Associate", sales1);
    
    std::cout << "Staff teams and chains created." << std::endl;

    // 3. Setup the Manager (Observer)
    StaffManager* manager = new StaffManager(dispatcher);
    std::cout << "StaffManager (Manager) created." << std::endl;

    // 4. Create a Plant (Rose)
    PlantSpeciesProfile* roseProfile = new FlowerProfile("Rose", "A beautiful red rose", "Full sun, well-drained soil");
    PlantProduct rose("rose_01", roseProfile);
    rose.setObserver(manager); // Register the manager to observe this plant

    // Add strategies to the rose
    rose.addStrategy("Watering", new DripWateringStrategy());
    rose.addStrategy("Pruning", new StandardPruningStrategy());
    
    std::cout << "\n--- A new Rose has been planted. ---" << std::endl;
    rose.transitionTo(new PlantedState());
    std::cout << "Current state: " << rose.getCurrentStateName() << std::endl;

    std::cout << "\n--- Time passes. The rose needs water. ---" << std::endl;
    // The plant's state changes, triggering the whole notification-command-dispatch flow
    rose.transitionTo(new NeedsWaterState());

    std::cout << "\n--- The rose has been watered. It continues to grow. ---" << std::endl;
    rose.advanceLifecycle(); // Moves from NeedsWaterState to InNurseryState
    std::cout << "Current state: " << rose.getCurrentStateName() << std::endl;

    // 5. Create another Plant (Succulent)
    PlantSpeciesProfile* succulentProfile = new SucculentProfile("Echeveria", "A drought-tolerant succulent", "Bright, indirect light, infrequent watering");
    PlantProduct succulent("succulent_01", succulentProfile);
    succulent.setObserver(manager);

    // Add strategies to the succulent
    succulent.addStrategy("Watering", new GentleMistStrategy());
    succulent.addStrategy("Fertilizing", new FertilizingStrategy());

    std::cout << "\n--- A new Succulent has been potted. ---" << std::endl;
    succulent.transitionTo(new PlantedState());
    std::cout << "Current state: " << succulent.getCurrentStateName() << std::endl;

    std::cout << "\n--- Time passes. The succulent looks pale and needs nutrients. ---" << std::endl;
    succulent.transitionTo(new NeedsFertilizerState());

    std::cout << "\n--- The succulent has been fertilized. It continues to grow. ---" << std::endl;
    succulent.advanceLifecycle();
    std::cout << "Current state: " << succulent.getCurrentStateName() << std::endl;


    std::cout << "\n--- Cleaning up. ---" << std::endl;
    // In a real app, command prototypes and staff would need proper memory management.
    // This is a simplified cleanup for demonstration.
    delete dispatcher;
    delete manager;
    delete gardener1;
    delete gardener2;
    delete gardener3;
    delete gardener4;
    delete sales1;
    delete sales2;
    delete roseProfile;
    delete succulentProfile;

    return 0;
}
