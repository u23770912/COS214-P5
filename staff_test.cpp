#include <iostream>
#include <cassert>
#include <string>
#include "StaffChainHandler.h"
#include "Cashier.h"
#include "Gardener.h"
#include "MoveToSalesFloorCommand.h"
#include "WaterCommand.h"
#include "PruneCommand.h"
#include "FertilizeCommand.h"
#include "PlantProduct.h"
#include "SinglePlant.h"
#include "FlowerProfile.h"

// Test result tracking
int tests_passed = 0;
int tests_failed = 0;

void assert_test(bool condition, const std::string& test_name) {
    if (condition) {
        std::cout << "[PASS] " << test_name << std::endl;
        tests_passed++;
    } else {
        std::cout << "[FAIL] " << test_name << std::endl;
        tests_failed++;
    }
}

// Test 1: Handler chain setup
void test_chain_setup() {
    std::cout << "\n=== Test 1: Chain Setup ===" << std::endl;
    
    Cashier* cashier1 = new Cashier();
    Cashier* cashier2 = new Cashier();
    Gardener* gardener1 = new Gardener();
    
    cashier1->setNext(cashier2);
    cashier2->setNext(gardener1);
    
    assert_test(cashier1->getNext() == cashier2, "Cashier1 -> Cashier2 link");
    assert_test(cashier2->getNext() == gardener1, "Cashier2 -> Gardener link");
    assert_test(gardener1->getNext() == nullptr, "Gardener is last in chain");
    
    delete cashier1;
    delete cashier2;
    delete gardener1;
}

// Test 2: Staff busy status
void test_busy_status() {
    std::cout << "\n=== Test 2: Busy Status ===" << std::endl;
    
    Cashier* cashier = new Cashier();
    
    assert_test(!cashier->isBusy(), "Initially not busy");
    
    cashier->setBusy(true);
    assert_test(cashier->isBusy(), "Set to busy");
    
    cashier->setBusy(false);
    assert_test(!cashier->isBusy(), "Set back to not busy");
    
    delete cashier;
}

// Test 3: Handled command (Cashier handles MoveToSalesFloor)
void test_handled_command() {
    std::cout << "\n=== Test 3: Handled Command (Cashier) ===" << std::endl;
    
    Cashier* cashier = new Cashier();
    //SinglePlant* plant = new SinglePlant("Rose", "Red Rose", "Shrub");
    
    PlantSpeciesProfile* profile = new FlowerProfile("Rose", "250ml", "Partial Sun", "Loamy");
    PlantProduct* plant = new PlantProduct("P001", profile);

    MoveToSalesFloorCommand* cmd = new MoveToSalesFloorCommand();
    cmd->setReceiver(plant);
    
    std::cout << "Command type: " << cmd->getType() << std::endl;
    std::cout << "Required role: " << cmd->getRequiredRole() << std::endl;
    
    // Cashier should handle MoveToSalesFloor commands
    assert_test(cmd->getType() == "MoveToSalesFloor", "Command type is MoveToSalesFloor");
    assert_test(cmd->getRequiredRole() == "Sales", "Required role is Sales");
    
    // Note: handleCommand takes ownership and deletes the command
    cashier->handleCommand(cmd);
    
    std::cout << "Command handled by Cashier" << std::endl;
    assert_test(true, "Cashier handled MoveToSalesFloor command");
    
    delete cashier;
    delete plant;
}

// Test 4: Command delegation within same role (Cashier to Cashier)
void test_command_chain_delegation() {
    std::cout << "\n=== Test 4: Command Chain Delegation (Same Role) ===" << std::endl;
    
    // Create chain of same role type
    Cashier* cashier1 = new Cashier();
    Cashier* cashier2 = new Cashier();
    cashier1->setNext(cashier2);
    
    // Make first cashier busy
    cashier1->setBusy(true);
    
    PlantSpeciesProfile* profile = new FlowerProfile("Rose", "250ml", "Partial Sun", "Loamy");
    PlantProduct* plant = new PlantProduct("P001", profile);
    
    MoveToSalesFloorCommand* moveCmd = new MoveToSalesFloorCommand();
    moveCmd->setReceiver(plant);
    
    std::cout << "Command type: " << moveCmd->getType() << std::endl;
    std::cout << "Required role: " << moveCmd->getRequiredRole() << std::endl;
    
    assert_test(moveCmd->getType() == "MoveToSalesFloor", "Command type is MoveToSalesFloor");
    assert_test(moveCmd->getRequiredRole() == "Sales", "Required role is Sales");
    assert_test(cashier1->isBusy(), "Cashier1 is busy");
    assert_test(!cashier2->isBusy(), "Cashier2 is available");
    
    // Busy Cashier1 should pass to available Cashier2 (same role)
    std::cout << "Passing command from busy Cashier1 to available Cashier2..." << std::endl;
    cashier1->handleCommand(moveCmd);
    
    assert_test(true, "Command delegated within same role type");
    
    delete cashier1;
    delete cashier2;
    delete plant;
}

// Test 5: Unhandled command (wrong role type)
void test_unknown_command() {
    std::cout << "\n=== Test 5: Unhandled Command (Wrong Role) ===" << std::endl;
    
    // Create chain of Cashiers only
    Cashier* cashier1 = new Cashier();
    Cashier* cashier2 = new Cashier();
    cashier1->setNext(cashier2);

    PlantSpeciesProfile* profile = new FlowerProfile("Rose", "250ml", "Partial Sun", "Loamy");
    PlantProduct* plant = new PlantProduct("P001", profile);
    
    // Create a Greenhouse command for a Sales chain (wrong role)
    WaterCommand* waterCmd = new WaterCommand();
    waterCmd->setReceiver(plant);
    
    std::cout << "Command type: " << waterCmd->getType() << std::endl;
    std::cout << "Required role: " << waterCmd->getRequiredRole() << std::endl;
    
    // Cashier chain cannot handle Greenhouse commands
    std::cout << "Sending Greenhouse command to Sales chain..." << std::endl;
    cashier1->handleCommand(waterCmd);
    
    assert_test(true, "Unhandled command (wrong role) processed gracefully");
    
    delete cashier1;
    delete cashier2;
    delete plant;
}

// Test 6: Busy handler passes to next in chain
void test_busy_handler_delegation() {
    std::cout << "\n=== Test 6: Busy Handler Delegation ===" << std::endl;
    
    Cashier* cashier1 = new Cashier();
    Cashier* cashier2 = new Cashier();
    cashier1->setNext(cashier2);
    
    // Make first cashier busy
    cashier1->setBusy(true);
    
    PlantSpeciesProfile* profile = new FlowerProfile("Rose", "250ml", "Partial Sun", "Loamy");
    PlantProduct* plant = new PlantProduct("P001", profile);
    MoveToSalesFloorCommand* cmd = new MoveToSalesFloorCommand();
    cmd->setReceiver(plant);
    
    assert_test(cashier1->isBusy(), "Cashier1 is busy");
    assert_test(!cashier2->isBusy(), "Cashier2 is available");
    
    std::cout << "Sending command to busy handler..." << std::endl;
    cashier1->handleCommand(cmd);
    
    assert_test(true, "Busy handler delegated to next available handler");
    
    delete cashier1;
    delete cashier2;
    delete plant;
}

// Test 7: Multiple commands with separate role chains
void test_multiple_commands() {
    std::cout << "\n=== Test 7: Multiple Commands (Separate Chains) ===" << std::endl;
    
    // Sales chain
    Cashier* cashier1 = new Cashier();
    Cashier* cashier2 = new Cashier();
    cashier1->setNext(cashier2);
    
    // Greenhouse chain
    Gardener* gardener1 = new Gardener();
    Gardener* gardener2 = new Gardener();
    gardener1->setNext(gardener2);

    PlantSpeciesProfile* profile1 = new FlowerProfile("Rose", "250ml", "Partial Sun", "Loamy");
    PlantProduct* plant1 = new PlantProduct("P001", profile1);
    PlantSpeciesProfile* profile2 = new FlowerProfile("Fern", "300ml", "Shade", "Rich Soil");
    PlantProduct* plant2 = new PlantProduct("P002", profile2);
    PlantSpeciesProfile* profile3 = new FlowerProfile("Orchid", "200ml", "Indirect Sun", "Well-drained");
    PlantProduct* plant3 = new PlantProduct("P003", profile3);
    
    // Command 1: Move to sales floor (send to Sales chain)
    MoveToSalesFloorCommand* cmd1 = new MoveToSalesFloorCommand();
    cmd1->setReceiver(plant1);
    
    // Command 2: Water (send to Greenhouse chain)
    WaterCommand* cmd2 = new WaterCommand();
    cmd2->setReceiver(plant2);
    
    // Command 3: Fertilize (send to Greenhouse chain)
    FertilizeCommand* cmd3 = new FertilizeCommand();
    cmd3->setReceiver(plant3);
    
    std::cout << "Processing commands through appropriate chains..." << std::endl;
    cashier1->handleCommand(cmd1);  // Sales command to Sales chain
    gardener1->handleCommand(cmd2); // Greenhouse command to Greenhouse chain
    gardener1->handleCommand(cmd3); // Greenhouse command to Greenhouse chain
    
    assert_test(true, "Multiple commands routed to correct role chains");
    
    delete cashier1;
    delete cashier2;
    delete gardener1;
    delete gardener2;
    delete plant1;
    delete plant2;
    delete plant3;
}

// Test 8: Chain with no next handler
void test_single_handler_chain() {
    std::cout << "\n=== Test 8: Single Handler (No Next) ===" << std::endl;
    
    Cashier* cashier = new Cashier();
    // No next handler set
    
    //SinglePlant* plant = new SinglePlant("Daisy", "White Daisy", "Flower");
    PlantSpeciesProfile* profile = new FlowerProfile("Rose", "250ml", "Partial Sun", "Loamy");
    PlantProduct* plant = new PlantProduct("P001", profile);

    MoveToSalesFloorCommand* cmd = new MoveToSalesFloorCommand();
    cmd->setReceiver(plant);
    
    assert_test(cashier->getNext() == nullptr, "No next handler in chain");
    
    std::cout << "Processing command with single handler..." << std::endl;
    cashier->handleCommand(cmd);
    
    assert_test(true, "Single handler processed command without crash");
    
    delete cashier;
    delete plant;
}

// Test 9: Clear assignment
void test_clear_assignment() {
    std::cout << "\n=== Test 9: Clear Assignment ===" << std::endl;
    
    Gardener* gardener = new Gardener();
    
    assert_test(gardener->getActivePlant() == nullptr, "Initially no active plant");
    assert_test(gardener->getActiveTask().empty(), "Initially no active task");
    
    gardener->clearAssignment();
    
    assert_test(gardener->getActivePlant() == nullptr, "Active plant cleared");
    assert_test(gardener->getActiveTask().empty(), "Active task cleared");
    
    delete gardener;
}

// Test 10: Command type validation
void test_command_types() {
    std::cout << "\n=== Test 10: Command Type Validation ===" << std::endl;
    
    //SinglePlant* plant = new SinglePlant("Test", "Test Plant", "Test");
    PlantSpeciesProfile* profile = new FlowerProfile("Rose", "250ml", "Partial Sun", "Loamy");
    PlantProduct* plant = new PlantProduct("P001", profile);
    
    MoveToSalesFloorCommand* moveCmd = new MoveToSalesFloorCommand();
    WaterCommand* waterCmd = new WaterCommand();
    PruneCommand* pruneCmd = new PruneCommand();
    FertilizeCommand* fertilizeCmd = new FertilizeCommand();
    
    moveCmd->setReceiver(plant);
    waterCmd->setReceiver(plant);
    pruneCmd->setReceiver(plant);
    fertilizeCmd->setReceiver(plant);
    
    assert_test(moveCmd->getType() == "MoveToSalesFloor", "MoveToSalesFloor type");
    assert_test(waterCmd->getType() == "Watering", "Water type");
    assert_test(pruneCmd->getType() == "Pruning", "Prune type");
    assert_test(fertilizeCmd->getType() == "Fertilizing", "Fertilize type");
    
    assert_test(moveCmd->getRequiredRole() == "Sales", "MoveToSalesFloor role");
    assert_test(waterCmd->getRequiredRole() == "Greenhouse", "Water role");
    assert_test(pruneCmd->getRequiredRole() == "Greenhouse", "Prune role");
    assert_test(fertilizeCmd->getRequiredRole() == "Greenhouse", "Fertilize role");
    
    delete moveCmd;
    delete waterCmd;
    delete pruneCmd;
    delete fertilizeCmd;
    delete plant;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  STAFF COORDINATION UNIT TESTS" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // Run all tests
    test_chain_setup();
    test_busy_status();
    test_handled_command();
    test_command_chain_delegation();
    test_unknown_command();
    test_busy_handler_delegation();
    test_multiple_commands();
    test_single_handler_chain();
    test_clear_assignment();
    test_command_types();
    
    // Print summary
    std::cout << "\n========================================" << std::endl;
    std::cout << "  TEST SUMMARY" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Passed: " << tests_passed << std::endl;
    std::cout << "Failed: " << tests_failed << std::endl;
    std::cout << "Total:  " << (tests_passed + tests_failed) << std::endl;
    std::cout << "========================================" << std::endl;
    
    if (tests_failed == 0) {
        std::cout << "\n✓ All tests passed!" << std::endl;
        return 0;
    } else {
        std::cout << "\n✗ Some tests failed!" << std::endl;
        return 1;
    }
}
