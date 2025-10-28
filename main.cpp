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
<<<<<<< HEAD
    StaffChainHandler* gardener = new Gardener();
    StaffChainHandler* cashier = new Cashier();
    gardener->setNext(cashier);
    
    // Create StaffMember that wraps the chain
    StaffMember* dispatcher = new StaffMember();
    dispatcher->registerTeam("Greenhouse", gardener);
    dispatcher->registerTeam("Sales", cashier);
=======
    GreenhouseStaff* gardener1 = new Gardener();
    GreenhouseStaff* gardener2 = new Gardener();
    GreenhouseStaff* gardener3 = new Gardener();
    SalesFloorStaff* cashier1 = new Cashier();
    SalesFloorStaff* cashier2 = new Cashier();
    gardener1->setNext(gardener2);
    gardener2->setNext(gardener3);
    
    cashier1->setNext(cashier2);
    
    // Create StaffMember that wraps the chain
    StaffMember* dispatcher = new StaffMember();
    dispatcher->registerTeam("Greenhouse", gardener1);
    dispatcher->registerTeam("Sales", cashier1);
>>>>>>> a5cc157f06bf6b3c50e503d2c0883ab9968a4990
    
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
    
<<<<<<< HEAD
=======
    std::cout << "\n9. Plant notifies with 'ReadyForSale'..." << std::endl;
    plant->notify("ReadyForSale");
    // (User will be prompted for input inside processUpdate)

>>>>>>> a5cc157f06bf6b3c50e503d2c0883ab9968a4990
    // Cleanup
    delete plant;
    delete manager;
    delete dispatcher;
<<<<<<< HEAD
    delete cashier;
    delete gardener;
    
=======
    delete cashier1;
    delete cashier2;
    delete gardener1;
    delete gardener2;
    delete gardener3;

>>>>>>> a5cc157f06bf6b3c50e503d2c0883ab9968a4990
    std::cout << "\n=== TEST COMPLETED ===" << std::endl;
    return 0;
}