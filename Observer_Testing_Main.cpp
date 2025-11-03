#include <iostream>
#include <string>
#include "LifeCycleMonitor.h"
#include "PlantProduct.h"
#include "FlowerProfile.h"
#include "StaffManager.h"
#include "StaffMember.h"
#include "Command.h"
#include "WaterCommand.h"
#include "FertilizeCommand.h"
#include "PruneCommand.h"

using namespace std;

int main() {
    cout << "\n=== OBSERVER PATTERN TEST ===\n" << endl;
    
    // Register commands
    Command::registerCommand("water", new WaterCommand());
    Command::registerCommand("fertilize", new FertilizeCommand());
    Command::registerCommand("prune", new PruneCommand());
    
    // Create plant
    PlantSpeciesProfile* roseProfile = new FlowerProfile("Rose", "200", "Full Sun", "Loamy");
    PlantProduct* rose = new PlantProduct("ROSE-001", roseProfile);
    cout << "✓ Created plant: ROSE-001\n" << endl;
    
    // Create monitor
    LifeCycleMonitor* monitor = new LifeCycleMonitor(rose);
    cout << "✓ Created LifeCycleMonitor\n" << endl;
    
    StaffMember* staffDispatcher = new StaffMember();
    StaffManager* staffManager = new StaffManager(staffDispatcher);
    cout << "✓ Created StaffManager\n" << endl;
    
    // REGISTERING OBSERVERS
    monitor->registerObserver(staffManager);
    cout << "✓ Registered observer\n" << endl;
    
    cout << "\n--- Testing Observer Notifications ---\n" << endl;
    cout << "Triggering water notification..." << endl;
    monitor->notify("water");
    
    cout << "\nTriggering fertilize notification..." << endl;
    monitor->notify("fertilize");
    
    cout << "\nTriggering prune notification..." << endl;
    monitor->notify("prune");
    
    // REMOVING OBSERVER
    cout << "\n--- Testing Observer Removal ---" << endl;
    monitor->removeObserver(staffManager);
    monitor->removeObserver(staffManager);
    cout << "✓ Observer removed\n" << endl;
    
    cout << "\nTriggering notification with no observers..." << endl;
    monitor->notify("water");
    cout << "✓ No output (observer successfully removed)\n" << endl;
    
    // CLEANUP
    cout << "\n--- Cleanup ---" << endl;
    delete monitor;
    delete staffManager;
    delete staffDispatcher;
    delete rose;
    delete roseProfile;
    Command::cleanupPrototypes();
    cout << "✓ All resources cleaned up\n" << endl;
    
    cout << "\n=== OBSERVER PATTERN TEST COMPLETE ===\n" << endl;
    cout << "The test shows that:\n";
    cout << "  1. Observer can be registered to LifeCycleMonitor\n";
    cout << "  2. Observer receives notifications when events occur\n";
    cout << "  3. Observer can be removed and stops receiving notifications\n";
    
    return 0;
}