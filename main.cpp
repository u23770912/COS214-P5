// ============= Autonomous Multiplex Simulation Main =============
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "PlantProduct.h"
#include "StaffManager.h"
#include "AutonomousMode.h"
#include "CareCommand.h"
#include "WaterCommand.h"
#include "PruneCommand.h"
#include "FertilizeCommand.h"
#include "StaffMember.h"
#include "StaffChainHandler.h"
#include "Gardener.h"
#include "Cashier.h"
#include "MoveToSalesFloorCommand.h"
#include "TerminalUI.h"

#include "TreeProfile.h"
#include "FlowerProfile.h"
#include "CactusProfile.h"
#include "SucculentProfile.h"

namespace {

class MapProfile : public PlantSpeciesProfile {
public:
    MapProfile(std::string name, std::map<std::string, std::string> properties)
        : speciesName(std::move(name)), propertyMap(std::move(properties)) {}

    std::string getSpeciesName() const override { return speciesName; }

    std::string getProperty(const std::string &key) const override {
        auto it = propertyMap.find(key);
        return it != propertyMap.end() ? it->second : "";
    }

private:
    std::string speciesName;
    std::map<std::string, std::string> propertyMap;
};

const std::map<std::string, int> kStateTargetSeconds = {
    {"Planted", 30},
    {"InNursery", 45},
    {"Growing", 90},
    {"ReadyForSale", 0},
    {"Withering", 0}
};

void registerCareCommands() {
    static bool registered = false;
    if (registered) {
        return;
    }

    CareCommand::registerCommand("Watering", new WaterCommand());
    CareCommand::registerCommand("Pruning", new PruneCommand());
    CareCommand::registerCommand("Fertilizing", new FertilizeCommand());
    CareCommand::registerCommand("MoveToSalesFloor", new MoveToSalesFloorCommand());
    CareCommand::registerCommand("ReadyForSale", new MoveToSalesFloorCommand());
    registered = true;
}

void displayStaffStatus(const std::vector<std::pair<std::string, StaffChainHandler *>> &staff) {
    TerminalUI::printSection("WORKFORCE STATUS");
    for (const auto &member : staff) {
        StaffChainHandler *handler = member.second;
        if (handler->isBusy()) {
            TerminalUI::printBusy(member.first);
            const PlantProduct *plant = handler->getActivePlant();
            std::string plantId = plant ? plant->getId() : std::string();
            std::string plantState = plant ? plant->getCurrentStateName() : std::string();
            std::string task = handler->getActiveTask();
            TerminalUI::printStaffAssignment(plantId, plantState, task);
        } else {
            TerminalUI::printAvailable(member.first);
        }
    }
    TerminalUI::printDivider();
}

void renderPlantVisualizer(const std::vector<PlantProduct *> &plants) {
    TerminalUI::printSection("PLANT VISUALIZER");
    for (auto *plant : plants) {
        std::string state = plant->getCurrentStateName();
        int secondsInState = plant->getSecondsInCurrentState();
        auto targetIt = kStateTargetSeconds.find(state);
        int targetSeconds = targetIt != kStateTargetSeconds.end() ? targetIt->second : 0;

        const int barWidth = 24;
        int filled = targetSeconds > 0 ? std::min(barWidth, (secondsInState * barWidth) / targetSeconds) : barWidth;
        std::string bar(barWidth, '.');
        for (int i = 0; i < filled; ++i) {
            bar[i] = '#';
        }

        std::ostringstream line;
        line << "[" << plant->getId() << "] "
             << std::setw(12) << std::left << plant->getProfile()->getSpeciesName()
             << " " << std::setw(15) << std::left << state 
             << " " << bar << " "
             << secondsInState << "s";
        std::cout << line.str() << std::endl;
    }
    TerminalUI::printDivider();
}

void displayPlantStrategyDetails(const std::vector<PlantProduct *> &plants) {
    TerminalUI::printSection("PLANT STRATEGY DETAILS & VALUES");
    
    for (auto *plant : plants) {
        std::string category = plant->getProfile()->getProperty("category");
        std::string species = plant->getProfile()->getSpeciesName();
        
        std::cout << "[" << plant->getId() << "] " << species << " (" << category << ")\n";
        
        // Display profile values that strategies will use
        std::cout << "  Profile Values:\n";
        std::cout << "    - Ideal Water: " << plant->getProfile()->getProperty("idealWater") << "\n";
        std::cout << "    - Ideal Sunlight: " << plant->getProfile()->getProperty("idealSunlight") << "\n";
        std::cout << "    - Ideal Soil: " << plant->getProfile()->getProperty("idealSoil") << "\n";
        
        // Display lifecycle properties
        std::cout << "    - Days in Nursery: " << plant->getProfile()->getProperty("daysInNursery") << "\n";
        std::cout << "    - Days to Ready: " << plant->getProfile()->getProperty("daysPlantedToReady") << "\n";
        std::cout << "    - Max Days Without Care: " << plant->getProfile()->getProperty("maxDaysWithoutCare") << "\n";
        
        // Display strategy assignments based on category
        std::cout << "  Available Strategies:\n";
        
        if (category == "Tree") {
            std::cout << "    - Watering: DripWateringStrategy (deep root watering)\n";
            std::cout << "    - Pruning: ArtisticPruningStrategy + StandardPruningStrategy\n";
            std::cout << "    - Fertilizing: FertilizingStrategy\n";
            std::cout << "    - NOT Available: Mist, Flood (trees need deep watering)\n";
        }
        else if (category == "Flower") {
            std::cout << "    - Watering: WateringStrategy (regular) + GentleMistStrategy (optional)\n";
            std::cout << "    - Pruning: StandardPruningStrategy (deadheading)\n";
            std::cout << "    - Fertilizing: FertilizingStrategy\n";
            std::cout << "    - NOT Available: Artistic pruning (not typical for flowers)\n";
        }
        else if (category == "Cactus" || category == "Succulent") {
            std::cout << "    - Watering: DripWateringStrategy (infrequent deep watering)\n";
            std::cout << "    - Fertilizing: FertilizingStrategy (light feeding)\n";
            std::cout << "    - NOT Available: Mist, Flood, Pruning (can cause rot/harm)\n";
        }
        
        std::cout << std::endl;
    }
    TerminalUI::printDivider();
}

void testCareStrategiesWithDetails(PlantProduct* plant) {
    std::string category = plant->getProfile()->getProperty("category");
    std::cout << "=== Strategy Test for " << plant->getProfile()->getSpeciesName() 
              << " (" << category << ") ===\n";
    
    std::vector<std::string> careTypes = {"water", "mist", "fertilize", "prune", "prune_artistic", "flood", "drip"};
    
    for (const auto& careType : careTypes) {
        std::cout << "  " << std::setw(15) << std::left << (careType + ":");
        
        // Show what would happen before actually performing care
        std::string normalized = careType;
        std::transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);
        
        // Check if strategy exists
        auto it = plant->getStrategyMap().find(normalized);
        
        if (it != plant->getStrategyMap().end()) {
            std::cout << "AVAILABLE - ";
            
            // Show expected values based on care type
            if (normalized == "water" || normalized == "mist" || normalized == "flood" || normalized == "drip") {
                std::string amount = plant->getProfile()->getProperty("idealWater");
                std::cout << "Uses: " << (amount.empty() ? "100ml (default)" : amount);
            } else if (normalized == "fertilize") {
                std::string amount = plant->getProfile()->getProperty("idealFertilizer");
                std::cout << "Uses: " << (amount.empty() ? "100g (default)" : amount + "g");
            } else if (normalized.find("prune") != std::string::npos) {
                std::string style = plant->getProfile()->getProperty("idealPruning");
                std::cout << "Style: " << (style.empty() ? "standard" : style);
            }
            
            std::cout << std::endl;
            
        } else {
            std::cout << "NOT AVAILABLE - ";
            
            // Explain why not available
            if (category == "Cactus" || category == "Succulent") {
                if (careType == "mist" || careType == "flood") {
                    std::cout << "Can cause rot in " << category << " plants";
                } else if (careType.find("prune") != std::string::npos) {
                    std::cout << "Rarely needed for " << category << " plants";
                } else {
                    std::cout << "Not appropriate for " << category << " plants";
                }
            } else if (category == "Tree" && careType == "mist") {
                std::cout << "Ineffective for deep tree roots";
            } else if (category == "Flower" && careType == "prune_artistic") {
                std::cout << "Not typical for flower plants";
            } else {
                std::cout << "No strategy assigned";
            }
            
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

void demonstrateStrategyExecution(PlantProduct* plant) {
    std::cout << "=== Strategy Execution Demo for " << plant->getProfile()->getSpeciesName() 
              << " (" << plant->getProfile()->getProperty("category") << ") ===\n";
    
    // Test only available strategies for this plant type
    std::vector<std::string> availableStrategies;
    std::string category = plant->getProfile()->getProperty("category");
    
    if (category == "Tree") {
        availableStrategies = {"water", "prune_artistic", "fertilize"};
    } else if (category == "Flower") {
        availableStrategies = {"water", "mist", "fertilize", "prune"};
    } else if (category == "Cactus" || category == "Succulent") {
        availableStrategies = {"water", "fertilize"};
    }
    
    for (const auto& strategy : availableStrategies) {
        std::cout << "Executing: " << std::setw(15) << std::left << strategy;
        plant->performCare(strategy);
    }
    std::cout << std::endl;
}

bool allPlantsSettled(const std::vector<PlantProduct *> &plants) {
    return std::all_of(plants.begin(), plants.end(), [](PlantProduct *plant) {
        std::string state = plant->getCurrentStateName();
        return state == "ReadyForSale" || state == "Withering";
    });
}

void displayStateTransitions(const std::vector<PlantProduct *> &plants, 
                           std::map<std::string, std::string>& previousStates) {
    TerminalUI::printSection("STATE TRANSITIONS");
    bool anyTransition = false;
    
    for (auto *plant : plants) {
        std::string currentState = plant->getCurrentStateName();
        std::string plantKey = plant->getId();
        
        if (previousStates[plantKey] != currentState) {
            if (!previousStates[plantKey].empty()) {
                TerminalUI::printSuccess("TRANSITION: " + plant->getId() + " " + 
                    plant->getProfile()->getSpeciesName() + " [" + 
                    previousStates[plantKey] + " → " + currentState + "]");
                anyTransition = true;
            }
            previousStates[plantKey] = currentState;
        }
    }
    
    if (!anyTransition) {
        TerminalUI::printInfo("No state changes in this cycle");
    }
    TerminalUI::printDivider();
}

} // namespace

int main() {
    TerminalUI::clearScreen();
    TerminalUI::printHeader("GREENHOUSE MANAGEMENT SYSTEM - STRATEGY & STATE TEST");

    TerminalUI::printSection("SYSTEM INITIALIZATION");
    TerminalUI::printInfo("Registering care command prototypes...");
    registerCareCommands();
    TerminalUI::printSuccess("Command prototypes ready");

    TerminalUI::printInfo("Configuring staff teams...");
    auto *gardener1 = new Gardener();
    auto *gardener2 = new Gardener();
    auto *gardener3 = new Gardener();
    auto *cashier1 = new Cashier();
    auto *cashier2 = new Cashier();

    gardener1->setNext(gardener2);
    gardener2->setNext(gardener3);
    gardener3->setNext(cashier1);
    cashier1->setNext(cashier2);

    std::vector<std::pair<std::string, StaffChainHandler *>> staff = {
        {"Gardener 1", gardener1},
        {"Gardener 2", gardener2},
        {"Gardener 3", gardener3},
        {"Cashier 1", cashier1},
        {"Cashier 2", cashier2}
    };

    auto *dispatcher = new StaffMember();
    dispatcher->registerTeam("Greenhouse", gardener1);
    dispatcher->registerTeam("Sales", cashier1);
    TerminalUI::printSuccess("Staff chain ready");

    TerminalUI::printInfo("Creating autonomous staff manager...");
    auto *manager = new StaffManager(dispatcher);
    manager->setMode(new AutonomousMode());
    TerminalUI::printSuccess("Manager running in autonomous mode");

    TerminalUI::printSection("PLANT SPECIMEN CREATION WITH PROFILES");
    std::vector<PlantProduct *> plants;
    std::vector<PlantSpeciesProfile *> profiles;

    // Create diverse plant types with specific profiles
    profiles.push_back(new FlowerProfile("Rose", "250ml", "Partial Sun", "Loamy"));
    profiles.push_back(new CactusProfile()); // Uses default cactus properties
    profiles.push_back(new TreeProfile("Bonsai", "180ml", "Full Sun", "Well-drained"));
    profiles.push_back(new SucculentProfile("Aloe Vera", "120ml", "Bright Indirect", "Sandy"));
    profiles.push_back(new TreeProfile("Oak Sapling", "300ml", "Full Sun", "Clay"));
    profiles.push_back(new FlowerProfile("Orchid", "200ml", "Shade", "Bark Mix"));

    int plantCounter = 1;
    for (auto *profile : profiles) {
        std::ostringstream id;
        id << "P" << std::setfill('0') << std::setw(3) << plantCounter++;
        auto *plant = new PlantProduct(id.str(), profile);
        plant->setObserver(manager);
        plants.push_back(plant);
        TerminalUI::printSuccess("Created " + plant->getId() + " - " + 
                               profile->getSpeciesName() + " (" + 
                               profile->getProperty("category") + ")");
    }

    TerminalUI::printSection("DETAILED STRATEGY ANALYSIS");
    displayPlantStrategyDetails(plants);

    TerminalUI::printSection("STRATEGY AVAILABILITY TEST");
    for (auto *plant : plants) {
        testCareStrategiesWithDetails(plant);
    }

    TerminalUI::printSection("STRATEGY EXECUTION DEMONSTRATION");
    for (auto *plant : plants) {
        demonstrateStrategyExecution(plant);
    }

    TerminalUI::printDivider();

    const int maxSimulationSeconds = 300;
    auto simulationStart = std::chrono::steady_clock::now();
    std::map<std::string, std::string> previousStates;

    // Initialize previous states
    for (auto *plant : plants) {
        previousStates[plant->getId()] = plant->getCurrentStateName();
    }

    int lastStrategyDisplay = -10; // Display strategies every 10 seconds

    while (true) {
        // Advance all plant lifecycles
        for (auto *plant : plants) {
            plant->advanceLifecycle();
        }

        auto now = std::chrono::steady_clock::now();
        int elapsedSeconds = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(now - simulationStart).count());

        TerminalUI::clearScreen();
        TerminalUI::printHeader("GREENHOUSE MANAGEMENT SYSTEM - LIVE SIMULATION");
        TerminalUI::printSection("SIMULATION CLOCK");
        TerminalUI::printInfo("Elapsed: " + std::to_string(elapsedSeconds) + "s (limit " + std::to_string(maxSimulationSeconds) + "s)");

        // Display state transitions
        displayStateTransitions(plants, previousStates);

        // Display plant visualization
        renderPlantVisualizer(plants);

        // Display strategy details periodically
        if (elapsedSeconds - lastStrategyDisplay >= 15) {
            displayPlantStrategyDetails(plants);
            lastStrategyDisplay = elapsedSeconds;
        }

        // Display staff status
        displayStaffStatus(staff);

        // Check termination conditions
        if (elapsedSeconds >= maxSimulationSeconds || allPlantsSettled(plants)) {
            TerminalUI::printWarning("Simulation stopping...");
            break;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    TerminalUI::printSection("FINAL SIMULATION SUMMARY");
    TerminalUI::printSubsection("PLANT FINAL STATES");
    for (auto *plant : plants) {
        std::string state = plant->getCurrentStateName();
        std::string status = (state == "ReadyForSale") ? "SUCCESS" : 
                            (state == "Withering") ? "FAILED" : "INCOMPLETE";
        
        std::cout << "[" << plant->getId() << "] " 
                  << std::setw(15) << std::left << plant->getProfile()->getSpeciesName()
                  << " State: " << std::setw(15) << std::left << state
                  << " Status: " << status << std::endl;
    }

    TerminalUI::printSubsection("STRATEGY EFFECTIVENESS BY PLANT TYPE");
    std::map<std::string, std::pair<int, int>> typeStats; // category -> {success, total}
    for (auto *plant : plants) {
        std::string category = plant->getProfile()->getProperty("category");
        std::string state = plant->getCurrentStateName();
        
        typeStats[category].second++; // total
        if (state == "ReadyForSale") {
            typeStats[category].first++; // success
        }
    }
    
    // C++11 compatible iteration
    for (std::map<std::string, std::pair<int, int>>::const_iterator it = typeStats.begin();
         it != typeStats.end(); ++it) {
        const std::string& category = it->first;
        const std::pair<int, int>& stats = it->second;
        double successRate = (stats.second > 0) ? (static_cast<double>(stats.first) / stats.second * 100) : 0;
        std::cout << category << ": " << stats.first << "/" << stats.second 
                  << " successful (" << std::fixed << std::setprecision(1) << successRate << "%)" << std::endl;
    }

    displayStaffStatus(staff);

    TerminalUI::printSection("CLEANUP");
    TerminalUI::printInfo("Shutting down system...");

    for (auto *plant : plants) {
        delete plant;
    }
    for (auto *profile : profiles) {
        delete profile;
    }

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