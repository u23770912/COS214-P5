#include "PlantProduct.h"
#include "PlantState.h"
#include "InNurseryState.h"
#include "PlantedState.h"
#include "ReadyForSaleState.h"
#include "WitheringState.h"
#include "PlantSpeciesProfile.h"
#include "FlowerProfile.h"
#include "CactusProfile.h"
#include "SucculentProfile.h"
#include "TreeProfile.h"
#include "CareStrategy.h"
#include "WateringStrategy.h"
#include "GentleMistStrategy.h"
#include "ArtisticPruningStrategy.h"
#include "FertilizingStrategy.h"
#include "FloodWateringStrategy.h"
#include "StandardPruningStrategy.h"
#include "DripWateringStrategy.h"
#include <iostream>
#include <string>  // For std::stoi

int main() {
    // Test Setup with FlowerProfile
    FlowerProfile* flowerProfile = new FlowerProfile("Rose", "100", "Full Sun", "Loamy");
    PlantProduct* flowerPlant = new PlantProduct("P001", flowerProfile);

    std::cout << "Initial State (Flower): " << flowerPlant->getCurrentStateName() << std::endl;

    // Test 1: Advance from InNursery to Planted (Flower)
    std::cout << "\n--- Test 1: Advance from InNursery to Planted (Flower) ---\n";
    flowerPlant->advanceLifecycle(); // Day 1
    flowerPlant->advanceLifecycle(); // Day 2
    flowerPlant->advanceLifecycle(); // Day 3
    std::cout << "State after 3 days: " << flowerPlant->getCurrentStateName() << std::endl;

    // Test 2: Advance from Planted to ReadyForSale (Flower)
    std::cout << "\n--- Test 2: Advance from Planted to ReadyForSale (Flower) ---\n";
    for (int i = 0; i < 5; ++i) flowerPlant->advanceLifecycle();
    std::cout << "State after 5 more days: " << flowerPlant->getCurrentStateName() << std::endl;

    // Test 3: Advance to Withering from ReadyForSale (Flower)
    std::cout << "\n--- Test 3: Advance to Withering from ReadyForSale (Flower) ---\n";
    for (int i = 0; i < 7; ++i) flowerPlant->advanceLifecycle();
    std::cout << "State after 7 more days: " << flowerPlant->getCurrentStateName() << std::endl;

    // Test 4: Manual transition to Withering (Flower)
    std::cout << "\n--- Test 4: Manual transition to Withering (Flower) ---\n";
    FlowerProfile* lavenderProfile = new FlowerProfile("Lavender", "80", "Partial Sun", "Sandy");
    PlantProduct* newFlowerPlant = new PlantProduct("P002", lavenderProfile);
    std::cout << "New Flower Plant Initial State: " << newFlowerPlant->getCurrentStateName() << std::endl;
    newFlowerPlant->transitionToWithering();
    std::cout << "State after manual withering: " << newFlowerPlant->getCurrentStateName() << std::endl;

    // Test 5: Watering Strategy (Flower)
    std::cout << "\n--- Test 5: Watering Strategy (Flower) ---\n";
    flowerPlant->performCare("water");

    // Test 6: Test with CactusProfile
    CactusProfile* cactusProfile = new CactusProfile();
    PlantProduct* cactusPlant = new PlantProduct("P003", cactusProfile);
    std::cout << "\nInitial State (Cactus): " << cactusPlant->getCurrentStateName() << std::endl;
    cactusPlant->advanceLifecycle(); // Day 1
    cactusPlant->advanceLifecycle(); // Day 2
    cactusPlant->advanceLifecycle(); // Day 3
    cactusPlant->advanceLifecycle(); // Day 4
    std::cout << "Cactus State after 4 days: " << cactusPlant->getCurrentStateName() << std::endl;
    cactusPlant->performCare("water");

    // ===== CLEANUP - MOVE ALL DELETES TO THE VERY END =====
    
    // Cleanup - do ALL deletions at the very end, after ALL operations
    delete flowerPlant;
    delete newFlowerPlant;
    delete cactusPlant;
    delete flowerProfile;
    delete lavenderProfile;  // Add this for the lavender plant
    delete cactusProfile;

    return 0;
}