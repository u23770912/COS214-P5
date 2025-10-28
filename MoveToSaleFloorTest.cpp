	#include <iostream>
	#include "PlantProduct.h"
	#include "BridgeDP/SucculentProfile.h"
	#include "StateDP/ReadyForSaleState.h"
	#include "StaffMember.h"
	#include "StaffManager.h"
	#include "Cashier.h"
	#include "MoveToSalesFloorCommand.h"
	#include "Command.h"
	#include "InventoryManager.h"

	int main() {
		using std::cout; using std::endl;

		// Register the MoveToSalesFloor command prototype
		Command::registerCommand("MoveToSalesFloor", new MoveToSalesFloorCommand());

		// Setup staff chain: two cashiers
		SalesFloorStaff* cashier1 = new Cashier();
		SalesFloorStaff* cashier2 = new Cashier();
		cashier1->setNext(cashier2);

		// Dispatcher (StaffMember) and register Sales team
		StaffMember* dispatcher = new StaffMember();
		dispatcher->registerTeam("Sales", cashier1);

		// StaffManager (observer) using the dispatcher
		StaffManager* manager = new StaffManager(dispatcher);
		// AutonomousMode is the default mode in StaffManager constructor

		// Create a simple plant profile and plant
		PlantSpeciesProfile* profile = new SucculentProfile("Aloe", "low", "bright", "sandy");
		PlantProduct* plant = new PlantProduct("T-100", profile);

    // Set the StaffManager as the plant's observer (so plant.notify will call manager->update)
        plant->setObserver(manager);		// Add plant to greenhouse inventory so the MoveToSalesFloorCommand can find and remove it
		InventoryManager& inventory = InventoryManager::getInstance();
		inventory.addToGreenhouse(plant);

		cout << "Before transition:" << endl;
		cout << "  Greenhouse size: " << inventory.getGreenHouseInventory().size() << endl;
		cout << "  ReadyForSale size: " << inventory.getReadyForSalePlants().size() << endl;

		// Transition plant to ReadyForSale - this should trigger the notification and move
		plant->transitionTo(new ReadyForSaleState());

		cout << "After transition:" << endl;
		cout << "  Greenhouse size: " << inventory.getGreenHouseInventory().size() << endl;
		cout << "  ReadyForSale size: " << inventory.getReadyForSalePlants().size() << endl;

		// Check result simply
		if (inventory.getReadyForSalePlants().size() > 0) {
			cout << "Test Result: Plant moved to sales floor (SUCCESS)" << endl;
		} else {
			cout << "Test Result: Plant NOT moved to sales floor (FAIL)" << endl;
		}

		// Cleanup allocated objects we own (avoid deleting plant because inventory keeps pointers)
		delete manager;
		delete dispatcher;
		delete cashier1;
		delete cashier2;

		// Note: We intentionally do not delete `plant` because InventoryManager stores pointers to it.
		return 0;
	}

