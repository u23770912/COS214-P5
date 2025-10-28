#include <iostream>
#include <string>
#include "PlantProduct.h"
#include "StaffManager.h"
#include "AutonomousMode.h"
#include "InteractiveMode.h"
#include "CareCommand.h"
#include "Command.h"
#include "WaterCommand.h"
#include "PruneCommand.h"
#include "FertilizeCommand.h"
#include "StaffMember.h"
#include "StaffChainHandler.h"
#include "Gardener.h"
#include "Cashier.h"
#include "MoveToSalesFloorCommand.h"

// Simple mock for PlantSpeciesProfile
class SimpleProfile : public PlantSpeciesProfile {
public:
    std::string getSpeciesName() const override { return "Rose"; }
    std::string getProperty(const std::string& key) const override { return "value"; }
};

int main() {
    std::cout << "=== SIMPLE TEST: Interactive Mode ===" << std::endl;
    
    // Register commands (Prototype Pattern)
    std::cout << "\n1. Registering commands..." << std::endl;
    CareCommand::registerCommand("Watering", new WaterCommand());
    CareCommand::registerCommand("Pruning", new PruneCommand());
    CareCommand::registerCommand("Fertilizing", new FertilizeCommand());
    CareCommand::registerCommand("MoveToSalesFloor", new MoveToSalesFloorCommand());
    CareCommand::registerCommand("ReadyForSale", new MoveToSalesFloorCommand());
    
    // Setup Chain of Responsibility
    std::cout << "\n2. Setting up chain of responsibility..." << std::endl;
    StaffChainHandler* gardener = new Gardener();
    StaffChainHandler* cashier = new Cashier();
    gardener->setNext(cashier);
    
    // Create StaffMember that wraps the chain
    StaffMember* dispatcher = new StaffMember();
    dispatcher->registerTeam("Greenhouse", gardener);
    dispatcher->registerTeam("Sales", cashier);
    
    // Setup Observer with Visitor (INTERACTIVE Mode)
    std::cout << "\n3. Setting up StaffManager with INTERACTIVE Mode..." << std::endl;
    StaffManager* manager = new StaffManager(dispatcher);
    manager->setMode(new InteractiveMode());
    
    // Setup Plant
    std::cout << "\n4. Creating plant and attaching observer..." << std::endl;
    PlantProduct* plant = new PlantProduct("P001", new SimpleProfile());
    plant->setObserver(manager);
    
    // TEST: Plant notifies -> Interactive mode handles everything
    std::cout << "\n5. Plant notifies with 'Watering'..." << std::endl;
    plant->notify("Watering");
    // (User will be prompted for input inside processUpdate)
    
    std::cout << "\n6. Plant notifies with 'Pruning'..." << std::endl;
    plant->notify("Pruning");
    // (User will be prompted for input inside processUpdate)
    
    std::cout << "\n7. Plant notifies with 'Fertilizing'..." << std::endl;
    plant->notify("Fertilizing");
    // (User will be prompted for input inside processUpdate)

    std::cout << "\n8. Plant notifies with 'ReadyForSale'..." << std::endl;
    plant->notify("ReadyForSale");
    // (User will be prompted for input inside processUpdate)
    
    // Cleanup
    delete plant;
    delete manager;
    delete dispatcher;
    delete cashier;
    delete gardener;
    
    std::cout << "\n=== TEST COMPLETED ===" << std::endl;
    return 0;
}