#include <iostream>
#include "GreenhouseSystem.h"
#include "PlantGroup.h"

int main() {
    // Create builder
    GreenhouseBuilder* greenhouseBuilder = new GreenhouseSystem();

    greenhouseBuilder->reset();

    // Start building manually (testing chain)
    greenhouseBuilder
        ->addCategory("Summer")
        ->addType("Shrub")
        ->addSpecies("Rose")
        ->addSpecies("Lavender")
        ->addCategory("Winter")
        ->addType("Tree")
        ->addSpecies("Pine");

    // Get final structure
    GreenhouseComponent* greenhouse = greenhouseBuilder->getGreenhouse();

    // Print to verify structure
    std::cout << "=== TEST GREENHOUSE STRUCTURE ===" << std::endl;
    greenhouse->print();

    // Cleanup
    delete greenhouseBuilder;
    return 0;
}


// #include <iostream>
// #include "GreenhouseSystem.h"
// #include "PlantGroup.h"

// int main() {
//     // Create the builder
//     GreenhouseBuilder* greenhouseBuilder = new GreenhouseSystem();

//     greenhouseBuilder->reset();

//     //  Build a simple structure manually
//     greenhouseBuilder
//         ->addCategory("Summer")
//         ->addSpecies("Rose")
//         ->addSpecies("Lavender")
//         ->addCategory("Winter")
//         ->addSpecies("Pine")
//         ->addSpecies("Snowdrop");

//     // Get the full greenhouse structure
//     GreenhouseComponent* greenhouse = greenhouseBuilder->getGreenhouse();

//     //  Print to verify
//     std::cout << "🌿 Greenhouse Structure:" << std::endl;
//     greenhouse->print();  // Assuming your PlantGroup implements a print() method

//     // Clean up
//     delete greenhouseBuilder;
//     return 0;
// }
