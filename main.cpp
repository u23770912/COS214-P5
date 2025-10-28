// ============= Enhanced Main with UI =============
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
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
#include "TerminalUI.h"

class SimpleProfile : public PlantSpeciesProfile {
public:
    std::string getSpeciesName() const override { return "Rose"; }
    std::string getProperty(const std::string& key) const override { return "value"; }
};

void displayStaffStatus(const std::vector<std::pair<std::string, StaffChainHandler*>>& staff) {
    TerminalUI::printSection("STAFF STATUS");
    for (const auto& member : staff) {
        TerminalUI::printStaffStatus(member.first, member.second->isBusy());
    }
    std::cout << std::endl;
}

int main() {
    TerminalUI::clearScreen();
    TerminalUI::printHeader("GREENHOUSE MANAGEMENT SYSTEM");
    
    // Register commands
    TerminalUI::printSection("SYSTEM INITIALIZATION");
    TerminalUI::printInfo("Registering commands...");
    Command::registerCommand("Watering", new WaterCommand());
    Command::registerCommand("Pruning", new PruneCommand());
    Command::registerCommand("Fertilizing", new FertilizeCommand());
    Command::registerCommand("MoveToSalesFloor", new MoveToSalesFloorCommand());
    Command::registerCommand("ReadyForSale", new MoveToSalesFloorCommand());
    TerminalUI::printSuccess("Commands registered");
    
    // Setup Chain of Responsibility
    TerminalUI::printInfo("Setting up staff chain...");
    GreenhouseStaff* gardener1 = new Gardener();
    GreenhouseStaff* gardener2 = new Gardener();
    GreenhouseStaff* gardener3 = new Gardener();
    SalesFloorStaff* cashier1 = new Cashier();
    SalesFloorStaff* cashier2 = new Cashier();
    
    gardener1->setNext(gardener2);
    gardener2->setNext(gardener3);
    cashier1->setNext(cashier2);
    
    // Track staff for status display
    std::vector<std::pair<std::string, StaffChainHandler*>> staff = {
        {"Gardener 1", gardener1},
        {"Gardener 2", gardener2},
        {"Gardener 3", gardener3},
        {"Cashier 1", cashier1},
        {"Cashier 2", cashier2}
    };
    
    StaffMember* dispatcher = new StaffMember();
    dispatcher->registerTeam("Greenhouse", gardener1);
    dispatcher->registerTeam("Sales", cashier1);
    TerminalUI::printSuccess("Staff chain configured");
    
    // Setup StaffManager
    TerminalUI::printInfo("Configuring StaffManager (Interactive Mode)...");
    StaffManager* manager = new StaffManager(dispatcher);
    manager->setMode(new InteractiveMode());
    TerminalUI::printSuccess("StaffManager ready");
    
    // Setup Plant
    TerminalUI::printInfo("Creating plant...");
    PlantProduct* plant = new PlantProduct("P001", new SimpleProfile());
    //plant->setObserver(manager);
    TerminalUI::printSuccess("Plant 'P001' (Rose) created");
    
    std::cout << std::endl;
    TerminalUI::printDivider();
    
    // Test commands
    std::vector<std::string> commands = {"Watering", "Pruning", "Fertilizing", "ReadyForSale", "ReadyForSale"};
    
    for (size_t i = 0; i < commands.size(); i++) {
        std::cout << std::endl;
        displayStaffStatus(staff);
        
        TerminalUI::printSection("TASK " + std::to_string(i + 1) + " OF " + std::to_string(commands.size()));
        //TerminalUI::printInfo("Plant requires: " + BOLD + commands[i] + RESET);
        
        plant->notify(commands[i]);
        
        // Small delay to see the effect
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        TerminalUI::printDivider();
    }
    
    // Cleanup
    std::cout << std::endl;
    TerminalUI::printSection("CLEANUP");
    TerminalUI::printInfo("Shutting down system...");
    
    delete plant;
    delete manager;
    delete dispatcher;
    delete cashier1;
    delete cashier2;
    delete gardener1;
    delete gardener2;
    delete gardener3;
    
    TerminalUI::printSuccess("System shutdown complete");
    TerminalUI::printDivider();
    
    return 0;
}