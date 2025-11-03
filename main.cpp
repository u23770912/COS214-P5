/**
 * @file integrated_main.cpp
 * @brief Integrated Greenhouse Management System with Customer Order Simulation
 * 
 * This program demonstrates the complete end-to-end flow of the Greenhouse Management System:
 * 1. System initialization (staff setup, command registration)
 * 2. Greenhouse plant lifecycle simulation
 * 3. Customer order interaction and validation
 * 
 * The program follows C++98 standards with minimal C++11 features (threads, nullptr).
 * It maintains modular design while providing a unified execution flow.
 * 
 * Execution Flow:
 * ===============
 * Phase 1: System Setup
 *   - Register command prototypes (Watering, Pruning, Fertilizing, MoveToSalesFloor)
 *   - Create staff chains (Gardeners for greenhouse, Cashiers for sales)
 *   - Initialize inventory manager
 * 
 * Phase 2: Greenhouse Lifecycle Simulation
 *   - Create plants with different species profiles
 *   - Add plants to greenhouse inventory
 *   - Simulate plant lifecycle progression through states
 *   - Staff handles care commands via Chain of Responsibility
 *   - Plants transition to ReadyForSale state
 *   - Plants moved to sales floor
 * 
 * Phase 3: Customer Order Simulation
 *   - Customer views available plants
 *   - Customer builds order with plants and bundles
 *   - Automatic discount calculation
 *   - Order validation through staff chain
 *   - Payment processing
 *   - Order confirmation
 * 
 * Design Patterns Demonstrated:
 * ============================
 * - Chain of Responsibility (Staff command handling)
 * - Observer (Plant lifecycle monitoring)
 * - Facade (TerminalUI, OrderUIFacade)
 * - Singleton (InventoryManager)
 * - Command (Plant care operations)
 * - State (Plant lifecycle states)
 * - Builder & Director (Order construction)
 */

#include <algorithm>
#include <cctype>
#include <cfloat>
#include <chrono>
#include <climits>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

// Command and Staff Infrastructure
#include "Command.h"
#include "Cashier.h"
#include "FertilizeCommand.h"
#include "Gardener.h"
#include "MoveToSalesFloorCommand.h"
#include "PruneCommand.h"
#include "StaffChainHandler.h"
#include "StaffManager.h"
#include "StaffMember.h"
#include "WaterCommand.h"

// Plant Infrastructure
#include "PlantProduct.h"
#include "FlowerProfile.h"
#include "SucculentProfile.h"
#include "TreeProfile.h"
#include "InventoryManager.h"

// Greenhouse Structure and Visualization
#include "GreenhouseManager.h"
#include "GreenhouseComponent.h"
#include "PlantPot.h"
#include "GreenhouseGUI.h"
#include "NetworkServer.h"

// Customer Order Infrastructure
#include "Customer.h"
#include "Order.h"
#include "OrderUIFacade.h"
#include "ConcreteOrderBuilder.h"
#include "SinglePlant.h"
#include "PlantBundle.h"
#include "OrderValidationHandler.h"
#include "PaymentProcessHandler.h"
#include "NotificationHandler.h"
#include "BouquetSuggestionFactory.h"
#include "BouquetSuggestion.h"
// #include "OrderMemento.h"

// UI Infrastructure
#include "TerminalUI.h"

// ANSI Color Codes for enhanced UI
#define ANSI_RESET   "\033[0m"
#define ANSI_BOLD    "\033[1m"
#define ANSI_RED     "\033[31m"
#define ANSI_GREEN   "\033[32m"
#define ANSI_YELLOW  "\033[33m"
#define ANSI_BLUE    "\033[34m"
#define ANSI_MAGENTA "\033[35m"
#define ANSI_CYAN    "\033[36m"

// Forward declarations
struct StaffContext;
void runGreenhouseSimulation(StaffContext& staff);
void runIntegratedSimulation(StaffContext& staff, GreenhouseManager* ghManager,
                            std::vector<PlantSpeciesProfile*>& profiles,
                            std::vector<PlantProduct*>& plants);
void runCustomerOrderTest(StaffContext& staff);

/**
 * @struct StaffContext
 * @brief Encapsulates all staff-related objects for easy management
 * 
 * This structure holds references to the staff dispatcher, manager,
 * and all individual handlers in the chain. It ensures proper cleanup
 * and maintains the relationships between staff objects.
 */
struct StaffContext {
    StaffMember* dispatcher;
    StaffManager* manager;
    std::vector<StaffChainHandler*> handlers;
    std::vector<std::pair<std::string, StaffChainHandler*> > roster;
    
    StaffContext() : dispatcher(0), manager(0) {}
};

/**
 * @brief Register all command prototypes for the Prototype pattern
 * 
 * Commands are registered once and cloned when needed. This ensures
 * efficient command creation without repeated allocation.
 */
void registerCareCommands() {
    static bool registered = false;
    if (registered) {
        return;
    }

    Command::registerCommand("Watering", new WaterCommand());
    Command::registerCommand("Pruning", new PruneCommand());
    Command::registerCommand("Fertilizing", new FertilizeCommand());
    Command::registerCommand("MoveToSalesFloor", new MoveToSalesFloorCommand());
    registered = true;
    
    TerminalUI::printSuccess("Care command prototypes registered");
}

/**
 * @brief Create and configure the staff chain of responsibility
 * 
 * Creates two teams:
 * 1. Greenhouse team: 5 Gardeners (handle plant care commands)
 * 2. Sales team: 3 Cashiers (handle sales floor operations)
 * 
 * Each team forms a chain where commands are passed until handled.
 * The manager back-reference allows queueing of unhandled commands.
 * 
 * @return StaffContext with fully configured staff structure
 */
StaffContext createStaffContext() {
    StaffContext ctx;

    // ============================================================================
    // Initialize empty staff context - workers added via GUI
    // ============================================================================
    
    // Create minimal dispatcher
    ctx.dispatcher = new StaffMember();
    
    // Create staff manager
    ctx.manager = new StaffManager(ctx.dispatcher);

    TerminalUI::printSuccess("Staff system initialized (empty - add workers via GUI)");
    return ctx;
}

/**
 * @brief Create plant species profiles for the greenhouse
 * 
 * Each profile defines the care requirements and lifecycle characteristics
 * for a specific type of plant (Flower, Tree, or Succulent).
 * 
 * @return Vector of plant species profiles
 */
std::vector<PlantSpeciesProfile*> createProfiles() {
    std::vector<PlantSpeciesProfile*> profiles;
    profiles.push_back(new FlowerProfile("Rose", "250ml", "Partial Sun", "Loamy"));
    profiles.push_back(new TreeProfile("Bonsai", "180ml", "Full Sun", "Well-drained"));
    profiles.push_back(new SucculentProfile("Aloe Vera", "120ml", "Bright Indirect", "Sandy"));
    profiles.push_back(new TreeProfile("Oak Sapling", "300ml", "Full Sun", "Clay"));
    profiles.push_back(new FlowerProfile("Orchid", "200ml", "Shade", "Bark Mix"));
    profiles.push_back(new SucculentProfile("Echeveria", "100ml", "Full Sun", "Gritty Mix"));
    profiles.push_back(new TreeProfile("Maple", "350ml", "Full Sun", "Loamy"));
    profiles.push_back(new FlowerProfile("Tulip", "150ml", "Full Sun", "Well-drained"));
    return profiles;
}

/**
 * @brief Create plant products from species profiles
 * 
 * Each plant is given a unique ID and attached to the staff manager
 * as an observer to handle lifecycle events.
 * 
 * @param profiles Vector of species profiles
 * @param manager Staff manager to observe plant lifecycle
 * @return Vector of plant products
 */
std::vector<PlantProduct*> createPlants(const std::vector<PlantSpeciesProfile*>& profiles,
                                        StaffManager* manager) {
    std::vector<PlantProduct*> plants;
    int counter = 1;
    for (size_t i = 0; i < profiles.size(); ++i) {
        const PlantSpeciesProfile* profile = profiles[i];
        std::ostringstream id;
        id << "P" << std::setfill('0') << std::setw(3) << counter++;
        PlantProduct* plant = new PlantProduct(id.str(), const_cast<PlantSpeciesProfile*>(profile));
        plant->setObserver(manager);
        plants.push_back(plant);
    }
    return plants;
}

/**
 * @brief Display state transitions when plants change lifecycle states
 * 
 * Tracks state history and displays transitions in a formatted manner.
 * Only displays actual transitions (not repeated states).
 * 
 * @param plants Vector of plants to check
 * @param history Map tracking previous state of each plant
 */
void displayStateTransitions(const std::vector<PlantProduct*>& plants,
                             std::map<std::string, std::string>& history) {
    TerminalUI::printSection("STATE TRANSITIONS");
    bool transitionLogged = false;
    for (size_t i = 0; i < plants.size(); ++i) {
        const PlantProduct* plant = plants[i];
        const std::string& currentState = plant->getCurrentStateName();
        std::string plantId = plant->getId();
        if (history[plantId] != currentState) {
            if (!history[plantId].empty()) {
                TerminalUI::printSuccess(plantId + " " + plant->getProfile()->getSpeciesName() + " [" +
                                         history[plantId] + " -> " + currentState + "]");
            }
            history[plantId] = currentState;
            transitionLogged = true;
        }
    }
    if (!transitionLogged) {
        TerminalUI::printInfo("No state changes detected");
    }
    TerminalUI::printDivider();
}

/**
 * @brief Get the desired duration for a plant to remain in a specific state
 * 
 * Terminal states (ReadyForSale, Withering) have duration of 0.
 * Other states use the duration specified in the plant's species profile.
 * 
 * @param plant The plant to check
 * @param state The state name
 * @return Duration in seconds
 */
int desiredStateDuration(const PlantProduct* plant, const std::string& state) {
    if (state == "ReadyForSale" || state == "Withering") {
        return 0;
    }
    const PlantSpeciesProfile* profile = plant->getProfile();
    return profile ? profile->getStateDurationSeconds(state, 0) : 0;
}

/**
 * @brief Render visual progress bars for each plant's lifecycle state
 * 
 * Shows a progress bar indicating how long the plant has been in its
 * current state relative to the target duration.
 * 
 * @param plants Vector of plants to visualize
 */
void renderPlantVisualizer(const std::vector<PlantProduct*>& plants) {
    TerminalUI::printSection("PLANT VISUALIZER");
    const int barWidth = 24;
    for (size_t i = 0; i < plants.size(); ++i) {
        const PlantProduct* plant = plants[i];
        std::string state = plant->getCurrentStateName();
        int secondsInState = plant->getSecondsInCurrentState();
        int targetSeconds = desiredStateDuration(plant, state);
        int filled = (targetSeconds > 0)
                         ? std::min(barWidth, (secondsInState * barWidth) / targetSeconds)
                         : barWidth;
        std::string bar(barWidth, '.');
        for (int j = 0; j < filled; ++j) {
            bar[j] = '#';
        }
        std::cout << "[" << plant->getId() << "] "
                  << std::setw(12) << std::left << plant->getProfile()->getSpeciesName()
                  << " " << std::setw(14) << std::left << state
                  << " " << bar << " " << secondsInState << "s" << std::endl;
    }
    TerminalUI::printDivider();
}

/**
 * @brief Display current status of all staff members
 * 
 * Shows which staff members are busy and what tasks they're performing.
 * 
 * @param roster Vector of staff name/handler pairs
 */
void displayStaffStatus(const std::vector<std::pair<std::string, StaffChainHandler*> >& roster) {
    TerminalUI::printSection("WORKFORCE STATUS");
    for (size_t i = 0; i < roster.size(); ++i) {
        const std::string& label = roster[i].first;
        StaffChainHandler* handler = roster[i].second;
        if (handler->isBusy()) {
            TerminalUI::printBusy(label);
            const PlantProduct* plant = handler->getActivePlant();
            std::string task = handler->getActiveTask();
            if (plant) {
                std::string strategyName = plant->getStrategyNameForCareType(task);
                TerminalUI::printStaffAssignment(plant->getId(), task, strategyName);
            }
        } else {
            TerminalUI::printAvailable(label);
        }
    }
    TerminalUI::printDivider();
}

/**
 * @brief Check if all plants have reached ReadyForSale state
 * 
 * @param plants Vector of plants to check
 * @return true if all plants are ready for sale, false otherwise
 */
bool allPlantsReady(const std::vector<PlantProduct*>& plants) {
    for (size_t i = 0; i < plants.size(); ++i) {
        if (plants[i]->getCurrentStateName() != "ReadyForSale") {
            return false;
        }
    }
    return true;
}

/**
 * @brief Run the greenhouse plant lifecycle simulation
 * 
 * This function simulates the complete lifecycle of plants from creation
 * through various states until they reach the sales floor.
 * 
 * Flow:
 * 1. Create plant species profiles
 * 2. Create plant products and add to greenhouse
 * 3. Simulate lifecycle progression (1 second per iteration)
 * 4. Staff handles care commands via Chain of Responsibility
 * 5. Display live status updates
 * 6. Stop when all plants reach ReadyForSale or time limit reached
 * 
 * @param staff Staff context with configured teams
 */
void runGreenhouseSimulation(StaffContext& staff) {
    TerminalUI::printHeader("PHASE 1: GREENHOUSE LIFECYCLE SIMULATION");
    TerminalUI::printInfo("Starting with empty greenhouse - use GUI or commands to add plants...");
    
    // ============================================================================
    // Phase 1.1: Initialize empty plant vectors
    // ============================================================================
    std::vector<PlantSpeciesProfile*> profiles;  // Start empty
    std::vector<PlantProduct*> plants;  // Start empty
    
    TerminalUI::printInfo("Greenhouse initialized with 0 plants");
    TerminalUI::printInfo("Use GUI or commands to add plants and workers");

    TerminalUI::printInfo("Greenhouse initialized with 0 plants");
    TerminalUI::printInfo("Use GUI or commands to add plants and workers");

    // ============================================================================
    // Phase 1.1.5: Build and Visualize Greenhouse Structure
    // ============================================================================
    std::cout << "\n";
    TerminalUI::printInfo("Building greenhouse organizational structure...");
    
    GreenhouseManager* ghManager = new GreenhouseManager();
    ghManager->buildGreenhouseStructure();
    
    TerminalUI::printSuccess("Greenhouse structure built (empty - add plants via GUI)");
    
    // Display the empty greenhouse structure
    ghManager->displayGreenhouseStructure();
    
    std::cout << "\n" << ANSI_CYAN << "Press Enter to start lifecycle simulation..." << ANSI_RESET;
    std::cin.ignore();
    std::cin.get();
    std::cout << "\n";

    // ============================================================================
    // Phase 1.2: Simulate plant lifecycle
    // ============================================================================
    const int maxSimulationSeconds = 120;
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::map<std::string, std::string> stateHistory;
    
    // Initialize state history
    for (size_t i = 0; i < plants.size(); ++i) {
        stateHistory[plants[i]->getId()] = plants[i]->getCurrentStateName();
    }

    int loopCounter = 0;
    int allReadyCounter = 0;  // Track how long all plants have been ready
    const int moveGracePeriod = 10;  // Allow 10 extra seconds for move commands

    TerminalUI::printInfo("Starting greenhouse lifecycle simulation...");
    std::cout << std::endl;

    while (true) {
        // Advance plant lifecycles (triggers state transitions and care commands)
        for (size_t i = 0; i < plants.size(); ++i) {
            plants[i]->advanceLifecycle();
        }

        // Process unhandled command queue every 2 seconds
        // This attempts to re-dispatch queued commands when staff becomes available
        if (loopCounter % 2 == 0) {
            staff.dispatcher->processUnhandledQueue();
        }

        // Calculate elapsed time
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        int elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

        // Display live status
        TerminalUI::clearScreen();
        TerminalUI::printHeader("GREENHOUSE MANAGEMENT SYSTEM - LIVE VIEW");
        TerminalUI::printSection("SIMULATION CLOCK");
        TerminalUI::printInfo("Elapsed: " + std::to_string(elapsed) + "s (limit " +
                              std::to_string(maxSimulationSeconds) + "s)");

        displayStateTransitions(plants, stateHistory);
        renderPlantVisualizer(plants);
        displayStaffStatus(staff.roster);

        // Check if all plants are ready
        if (allPlantsReady(plants)) {
            allReadyCounter++;
            if (allReadyCounter == 1) {
                TerminalUI::printInfo("All plants reached ReadyForSale! Allowing time for sales floor transfer...");
                TerminalUI::printSection("CASHIER TEAM ACTIVITY");
                std::cout << ANSI_CYAN << "Cashier team is now moving plants to the sales floor...\n" << ANSI_RESET;
            }
            
            // Display cashier activity during grace period
            if (allReadyCounter > 0 && allReadyCounter <= moveGracePeriod) {
                // Show which cashiers are processing moves
                bool cashierActive = false;
                for (size_t i = 0; i < staff.roster.size(); ++i) {
                    StaffChainHandler* handler = staff.roster[i].second;
                    // Check if this is a cashier (Sales team members)
                    if (staff.roster[i].first.find("Cashier") != std::string::npos) {
                        if (handler->isBusy()) {
                            cashierActive = true;
                            const PlantProduct* plant = handler->getActivePlant();
                            if (plant) {
                                std::cout << ANSI_GREEN << "[ACTIVE] " << staff.roster[i].first 
                                         << " is moving " << plant->getId() << " (" 
                                         << plant->getProfile()->getSpeciesName() 
                                         << ") to sales floor" << ANSI_RESET << std::endl;
                            }
                        }
                    }
                }
                
                if (!cashierActive && allReadyCounter > 2) {
                    // Show progress update
                    int salesFloorCount = InventoryManager::getInstance().getReadyForSalePlants().size();
                    std::cout << ANSI_YELLOW << "[UPDATE] Sales floor inventory: " 
                             << salesFloorCount << " plants available" << ANSI_RESET << std::endl;
                }
            }
        }

        // Check termination conditions
        // Allow extra time after all plants are ready for MoveToSalesFloor commands to execute
        if (elapsed >= maxSimulationSeconds || allReadyCounter >= moveGracePeriod) {
            TerminalUI::printWarning("Simulation stopping...");
            break;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
        loopCounter++;
    }

    // ============================================================================
    // Phase 1.3: Display final simulation summary
    // ============================================================================
    TerminalUI::printSection("GREENHOUSE SIMULATION FINAL SUMMARY");
    for (size_t i = 0; i < plants.size(); ++i) {
        const PlantProduct* plant = plants[i];
        std::cout << "[" << plant->getId() << "] "
                  << std::setw(14) << std::left << plant->getProfile()->getSpeciesName()
                  << " -> " << plant->getCurrentStateName() << std::endl;
    }
    
    int readyForSale = InventoryManager::getInstance().getReadyForSalePlants().size();
    TerminalUI::printSuccess("Greenhouse simulation complete!");
    TerminalUI::printInfo("Plants ready for sale: " + std::to_string(readyForSale));
    std::cout << std::endl;
    
    // Cleanup greenhouse manager
    delete ghManager;
    TerminalUI::printInfo("Greenhouse structure cleaned up");
    
    // Keep profile pointers for cleanup later
    // Note: plants are now owned by InventoryManager, profiles need manual cleanup
}

// ============================================================================
// Input Validation Helper Functions (C++98 compatible)
// ============================================================================

/**
 * @brief Get validated integer input from user
 */
int getValidInteger(const std::string& prompt, int min = INT_MIN, int max = INT_MAX) {
    std::string input;
    int value;
    
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        // Remove leading/trailing whitespace
        input.erase(0, input.find_first_not_of(" \t"));
        input.erase(input.find_last_not_of(" \t") + 1);
        
        if (input.empty()) {
            std::cout << "Error: Please enter a valid number.\n";
            continue;
        }
        
        std::stringstream ss(input);
        if (ss >> value && ss.eof()) {
            if (value >= min && value <= max) {
                return value;
            } else {
                std::cout << "Error: Please enter a number between " << min << " and " << max << ".\n";
            }
        } else {
            std::cout << "Error: Please enter a valid integer number.\n";
        }
    }
}

/**
 * @brief Get validated string input from user
 */
std::string getValidString(const std::string& prompt) {
    std::string input;
    
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        // Remove leading/trailing whitespace
        input.erase(0, input.find_first_not_of(" \t"));
        input.erase(input.find_last_not_of(" \t") + 1);
        
        if (!input.empty()) {
            return input;
        }
        
        std::cout << "Error: Please enter a valid non-empty value.\n";
    }
}

/**
 * @brief Get validated choice from user
 */
char getValidChoice(const std::string& prompt, const std::string& validChoices) {
    std::string input;
    
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        // Remove leading/trailing whitespace and convert to lowercase
        input.erase(0, input.find_first_not_of(" \t"));
        input.erase(input.find_last_not_of(" \t") + 1);
        
        if (input.length() == 1) {
            char choice = std::tolower(input[0]);
            if (validChoices.find(choice) != std::string::npos) {
                return choice;
            }
        }
        
        std::cout << "Error: Please enter one of these options: ";
        for (size_t i = 0; i < validChoices.length(); i++) {
            std::cout << validChoices[i];
            if (i < validChoices.length() - 1) std::cout << ", ";
        }
        std::cout << "\n";
    }
}

/**
 * @brief Show animated loading bar for better UX
 */
void showLoadingBar(const std::string& task, int durationMs = 1000) {
    std::cout << ANSI_CYAN << task << ": " << ANSI_RESET;
    int bars = 30;
    for (int i = 0; i <= bars; i++) {
        std::cout << ANSI_GREEN << "█" << ANSI_RESET << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(durationMs / bars));
    }
    std::cout << " " << ANSI_GREEN << "✓" << ANSI_RESET << std::endl;
}

/**
 * @brief Display enhanced interactive customer menu with all options
 */
void displayCustomerMenu() {
    std::cout << "\n" << ANSI_CYAN << ANSI_BOLD;
    std::cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
    std::cout << "    ║                        MAIN MENU                                 ║\n";
    std::cout << "    ╠══════════════════════════════════════════════════════════════════╣\n";
    std::cout << ANSI_RESET;
    
    std::cout << "    ║  " << ANSI_GREEN << "1" << ANSI_RESET << " │ 🌺 Browse Available Plants                                  ║\n";
    std::cout << "    ║  " << ANSI_GREEN << "2" << ANSI_RESET << " │ 🛒 Add Single Plant to Cart                                 ║\n";
    std::cout << "    ║  " << ANSI_GREEN << "3" << ANSI_RESET << " │ 🎁 Create Custom Plant Bundle                               ║\n";
    std::cout << "    ║  " << ANSI_GREEN << "4" << ANSI_RESET << " │ 💐 Browse Event Bouquet Suggestions                         ║\n";
    std::cout << "    ║  " << ANSI_GREEN << "5" << ANSI_RESET << " │ 👀 View Current Order                                       ║\n";
    std::cout << "    ║  " << ANSI_GREEN << "6" << ANSI_RESET << " │ 💾 Save Order Snapshot (Memento)                            ║\n";
    std::cout << "    ║  " << ANSI_GREEN << "7" << ANSI_RESET << " │ ⏮  Restore Last Order Snapshot                              ║\n";
    std::cout << "    ║  " << ANSI_GREEN << "8" << ANSI_RESET << " │ 💳 Checkout & Payment                                       ║\n";
    std::cout << "    ║  " << ANSI_GREEN << "9" << ANSI_RESET << " │ 🚪 Exit                                                     ║\n";
    
    std::cout << ANSI_CYAN << ANSI_BOLD;
    std::cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
    std::cout << ANSI_RESET;
    std::cout << "\n    " << ANSI_YELLOW << "➤ Enter your choice: " << ANSI_RESET;
}

/**
 * @brief Display available plants in an enhanced formatted table with details
 */
void displayAvailablePlants() {
    std::vector<PlantProduct*> plants = InventoryManager::getInstance().getReadyForSalePlants();

    TerminalUI::clearScreen();
    std::cout << "\n" << ANSI_GREEN << ANSI_BOLD;
    std::cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
    std::cout << "    ║                    🌿 PLANT CATALOG 🌿                           ║\n";
    std::cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
    std::cout << ANSI_RESET << "\n";
    
    if (plants.empty()) {
        std::cout << ANSI_RED << "    ✗ No plants available at the moment.\n" << ANSI_RESET;
        std::cout << "\n    " << ANSI_CYAN << "Press Enter to continue..." << ANSI_RESET;
        std::cin.ignore();
        std::cin.get();
        return;
    }
    
    std::cout << ANSI_CYAN << "    Available Plants: " << plants.size() << "\n" << ANSI_RESET;
    std::cout << "    " << std::string(66, '-') << "\n";
    
    for (size_t i = 0; i < plants.size(); i++) {
        std::string name = plants[i]->getProfile()->getSpeciesName();
        std::string water = plants[i]->getProfile()->getProperty("idealWater");
        std::string sun = plants[i]->getProfile()->getProperty("idealSunlight");
        
        std::cout << "    " << ANSI_YELLOW << std::setw(2) << (i+1) << ". " << ANSI_RESET;
        std::cout << ANSI_BOLD << std::setw(20) << std::left << name << ANSI_RESET;
        std::cout << " │ 💧 " << std::setw(12) << water;
        std::cout << " │ ☀️  " << std::setw(15) << sun;
        std::cout << " │ " << ANSI_GREEN << "$25.99" << ANSI_RESET << "\n";
    }
    
    std::cout << "    " << std::string(66, '-') << "\n";
    std::cout << "\n    " << ANSI_CYAN << "Press Enter to continue..." << ANSI_RESET;
    std::cin.ignore();
    std::cin.get();
}

/**
 * @brief Browse event-specific bouquet suggestions using Template Method Pattern
 */
void browseBouquetSuggestions() {
    TerminalUI::clearScreen();
    std::cout << "\n" << ANSI_MAGENTA << ANSI_BOLD;
    std::cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
    std::cout << "    ║              💐 EVENT BOUQUET SUGGESTIONS 💐                     ║\n";
    std::cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
    std::cout << ANSI_RESET << "\n";
    
    std::cout << "    [INFO] Bouquet suggestions feature temporarily disabled\n";
    std::cout << "\n    " << ANSI_CYAN << "Press Enter to return to menu..." << ANSI_RESET;
    std::cin.ignore();
    std::cin.get();
    
    /* Temporarily disabled - will be re-enabled when files are moved to root
    BouquetSuggestionFactory& factory = BouquetSuggestionFactory::getInstance();
    std::vector<std::string> events = factory.getAvailableEvents();
    
    std::cout << "    Available Events:\n";
    std::cout << "    " << std::string(66, '-') << "\n";
    for (size_t i = 0; i < events.size(); i++) {
        std::cout << "    " << ANSI_YELLOW << (i+1) << ". " << ANSI_RESET << events[i] << "\n";
    }
    std::cout << "    " << std::string(66, '-') << "\n";
    
    int choice = getValidInteger("\n    Select event (1-" + std::to_string(events.size()) + "): ", 1, events.size());
    
    std::string eventType = events[choice-1];
    BouquetSuggestionTemplate* tmpl = factory.getTemplate(eventType);
    
    if (!tmpl) {
        std::cout << ANSI_RED << "    ✗ Template not found!\n" << ANSI_RESET;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return;
    }
    
    TerminalUI::clearScreen();
    std::cout << "\n";
    std::vector<BouquetSuggestion> suggestions = tmpl->generateSuggestions();
    
    std::cout << "\n    " << ANSI_CYAN << "Press Enter to return to menu..." << ANSI_RESET;
    std::cin.ignore();
    std::cin.get();
    */
}

/**
 * @brief Run the enhanced interactive customer order experience
 * 
 * This function provides a comprehensive interactive menu-driven interface where customers can:
 * 1. Browse available plants from the sales floor with detailed information
 * 2. Add individual plants to their shopping cart
 * 3. Create custom plant bundles with automatic discounts
 * 4. Browse event-specific bouquet suggestions (Template Method Pattern)
 * 5. View their current order with full details
 * 6. Save order snapshots (Memento Pattern)
 * 7. Restore previous order states (Memento Pattern)
 * 8. Complete checkout with multiple payment options (Adapter Pattern)
 * 9. Exit when finished
 * 
 * Features:
 * - Enhanced UI with colors and progress indicators
 * - Robust input validation to prevent crashes
 * - Automatic discount calculation based on quantity
 * - Payment processing through Adapter Pattern (Cash/Credit/EFT)
 * - Integration with Cashier chain for command dispatch
 * - Order validation through Chain of Responsibility
 * - Inventory synchronization with greenhouse simulation
 * 
 * Design Patterns Demonstrated:
 * - Builder: Order construction
 * - Chain of Responsibility: Order validation, payment processing
 * - Observer: Customer interaction notifications
 * - Adapter: Multiple payment system integration
 * - Memento: Order state save/restore
 * - Template Method: Bouquet suggestions
 * - Facade: OrderUIFacade for automatic discounts
 * 
 * @param staff Staff context with configured teams (includes Cashier chain)
 */
void runCustomerOrderTest(StaffContext& staff) {
    // ============================================================================
    // Phase 2: Enhanced Customer Experience Header
    // ============================================================================
    TerminalUI::clearScreen();
    std::cout << "\n" << ANSI_GREEN << ANSI_BOLD;
    std::cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
    std::cout << "    ║                                                                  ║\n";
    std::cout << "    ║          🌿 GREENHOUSE NURSERY MANAGEMENT SYSTEM 🌿             ║\n";
    std::cout << "    ║                                                                  ║\n";
    std::cout << "    ║              Your Gateway to Green Living                       ║\n";
    std::cout << "    ║                                                                  ║\n";
    std::cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
    std::cout << ANSI_RESET << "\n";
    
    // ============================================================================
    // Phase 2.1: Display available inventory with loading animation
    // ============================================================================
    int availablePlants = InventoryManager::getInstance().getReadyForSalePlants().size();
    
    std::cout << ANSI_CYAN << "\n⚙  Verifying Greenhouse Inventory..." << ANSI_RESET << std::endl;
    showLoadingBar("Loading plant database", 800);
    
    std::cout << ANSI_GREEN << "✓ Inventory ready: " << availablePlants 
             << " plants available\n" << ANSI_RESET << std::endl;
    
    if (availablePlants == 0) {
        TerminalUI::printWarning("No plants available on sales floor!");
        TerminalUI::printInfo("Skipping customer order experience.");
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
        return;
    }
    
    // ============================================================================
    // Phase 2.2: Customer Registration
    // ============================================================================
    std::cout << ANSI_YELLOW << ANSI_BOLD;
    std::cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
    std::cout << "    ║                    CUSTOMER REGISTRATION                         ║\n";
    std::cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
    std::cout << ANSI_RESET << "\n";
    
    std::string name, email, phone;
    std::cout << "    " << ANSI_CYAN << "➤ Full Name: " << ANSI_RESET;
    std::getline(std::cin, name);
    std::cout << "    " << ANSI_CYAN << "➤ Email: " << ANSI_RESET;
    std::getline(std::cin, email);
    std::cout << "    " << ANSI_CYAN << "➤ Phone: " << ANSI_RESET;
    std::getline(std::cin, phone);
    
    Customer* customer = new Customer(name, email, phone);
    
    // Attach staff manager as observer to customer
    customer->attachObserver(static_cast<CustomerObserver*>(staff.manager));
    
    std::cout << "\n    " << ANSI_GREEN << "✓ Welcome, " << name << "! 🌿\n" << ANSI_RESET;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    // ============================================================================
    // Phase 2.3: Create order builder and memento support
    // ============================================================================
    ConcreteOrderBuilder* orderBuilder = new ConcreteOrderBuilder(customer->getName());
    Order* currentOrder = NULL;
    // OrderMemento* savedMemento = NULL;  // For memento pattern demonstration (temporarily disabled)
    
    // ============================================================================
    // Phase 2.4: Enhanced Interactive menu loop with extended features
    // ============================================================================
    bool customerActive = true;
    
    while (customerActive) {
        TerminalUI::clearScreen();
        std::cout << "\n" << ANSI_GREEN << ANSI_BOLD;
        std::cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
        std::cout << "    ║          🌿 GREENHOUSE NURSERY MANAGEMENT SYSTEM 🌿             ║\n";
        std::cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
        std::cout << ANSI_RESET;
        
        displayCustomerMenu();
        
        int choice;
        std::cin >> choice;
        
        // Check if input failed (e.g., user entered non-numeric input)
        if (std::cin.fail()) {
            std::cin.clear();  // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Discard invalid input
            std::cout << "\n    " << ANSI_RED << "✗ Invalid input! Please enter a number between 1-9.\n" << ANSI_RESET;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (choice < 1 || choice > 9) {
            std::cout << "\n    " << ANSI_RED << "✗ Invalid choice! Try again.\n" << ANSI_RESET;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }
        
        switch (choice) {
            case 1: {
                // Browse available plants with enhanced display
                displayAvailablePlants();
                break;
            }
            
            case 2: {
                // Add single plant to order with enhanced UI
                TerminalUI::clearScreen();
                std::cout << "\n" << ANSI_GREEN << ANSI_BOLD;
                std::cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
                std::cout << "    ║                    🌿 PLANT CATALOG 🌿                           ║\n";
                std::cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
                std::cout << ANSI_RESET << "\n";
                
                std::vector<PlantProduct*> plants = InventoryManager::getInstance().getReadyForSalePlants();
                
                if (plants.empty()) {
                    std::cout << ANSI_RED << "    ✗ No plants available!\n" << ANSI_RESET;
                    std::cout << "\n    " << ANSI_CYAN << "Press Enter to continue..." << ANSI_RESET;
                    std::cin.get();
                    break;
                }
                
                std::cout << ANSI_CYAN << "    Available Plants: " << plants.size() << "\n" << ANSI_RESET;
                std::cout << "    " << std::string(66, '-') << "\n";
                
                for (size_t i = 0; i < plants.size(); i++) {
                    std::string name_p = plants[i]->getProfile()->getSpeciesName();
                    std::string water = plants[i]->getProfile()->getProperty("idealWater");
                    std::string sun = plants[i]->getProfile()->getProperty("idealSunlight");
                    
                    std::cout << "    " << ANSI_YELLOW << std::setw(2) << (i+1) << ". " << ANSI_RESET;
                    std::cout << ANSI_BOLD << std::setw(20) << std::left << name_p << ANSI_RESET;
                    std::cout << " │ 💧 " << std::setw(12) << water;
                    std::cout << " │ ☀️  " << std::setw(15) << sun;
                    std::cout << " │ " << ANSI_GREEN << "$25.99" << ANSI_RESET << "\n";
                }
                std::cout << "    " << std::string(66, '-') << "\n";
                
                std::cout << "\n    " << ANSI_YELLOW << "➤ Enter plant number (1-" << plants.size() << "): " << ANSI_RESET;
                int plantNum;
                std::cin >> plantNum;
                
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << ANSI_RED << "    ✗ Invalid input! Please enter a number.\n" << ANSI_RESET;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    break;
                }
                std::cin.ignore();
                
                if (plantNum < 1 || plantNum > (int)plants.size()) {
                    std::cout << ANSI_RED << "    ✗ Invalid selection!\n" << ANSI_RESET;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    break;
                }
                
                std::cout << "    " << ANSI_YELLOW << "➤ Quantity: " << ANSI_RESET;
                int quantity;
                std::cin >> quantity;
                
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << ANSI_RED << "    ✗ Invalid input! Please enter a number.\n" << ANSI_RESET;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    break;
                }
                std::cin.ignore();
                
                if (quantity < 1) {
                    std::cout << ANSI_RED << "    ✗ Invalid quantity!\n" << ANSI_RESET;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    break;
                }
                
                // Create order if it doesn't exist
                if (!currentOrder) {
                    currentOrder = orderBuilder->getOrder();
                }
                
                // Add plant to order
                std::string plantType = plants[plantNum-1]->getProfile()->getSpeciesName();
                SinglePlant* plant = new SinglePlant(plantType, 25.99, quantity);
                currentOrder->addOrderItem(plant);
                
                std::cout << "\n    " << ANSI_GREEN << "✓ Added " << quantity << "x " << plantType << " to cart!\n" << ANSI_RESET;
                std::this_thread::sleep_for(std::chrono::milliseconds(800));
                break;
            }
            
            case 3: {
                // Create plant bundle with automatic discount
                TerminalUI::clearScreen();
                std::cout << "\n" << ANSI_MAGENTA << ANSI_BOLD;
                std::cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
                std::cout << "    ║                  🎁 CREATE CUSTOM BUNDLE 🎁                      ║\n";
                std::cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
                std::cout << ANSI_RESET << "\n";
                
                std::cout << "    " << ANSI_YELLOW << "➤ Bundle name: " << ANSI_RESET;
                std::string bundleName;
                std::getline(std::cin, bundleName);
                
                if (bundleName.empty()) {
                    std::cout << ANSI_RED << "    ✗ Bundle name cannot be empty!\n" << ANSI_RESET;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    break;
                }
                
                // Initial bundle with no discount (will be calculated automatically)
                PlantBundle* bundle = new PlantBundle(bundleName, "Custom", 1, 0.0);
                
                std::vector<PlantProduct*> plants = InventoryManager::getInstance().getReadyForSalePlants();
                
                if (plants.empty()) {
                    std::cout << ANSI_RED << "    ✗ No plants available for bundle!\n" << ANSI_RESET;
                    delete bundle;
                    std::cout << "\n    " << ANSI_CYAN << "Press Enter to continue..." << ANSI_RESET;
                    std::cin.get();
                    break;
                }
                
                // Display available plants
                std::cout << "\n    " << ANSI_CYAN << "Available Plants:\n" << ANSI_RESET;
                std::cout << "    " << std::string(66, '-') << "\n";
                for (size_t i = 0; i < plants.size(); i++) {
                    std::cout << "    " << ANSI_YELLOW << (i+1) << ". " << ANSI_RESET 
                             << plants[i]->getProfile()->getSpeciesName() << "\n";
                }
                std::cout << "    " << std::string(66, '-') << "\n";
                
                std::cout << "\n    " << ANSI_YELLOW << "➤ Number of plant types in bundle (1-10): " << ANSI_RESET;
                int numTypes;
                std::cin >> numTypes;
                
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << ANSI_RED << "    ✗ Invalid input! Please enter a number.\n" << ANSI_RESET;
                    delete bundle;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    break;
                }
                std::cin.ignore();
                
                if (numTypes < 1 || numTypes > 10) {
                    std::cout << ANSI_RED << "    ✗ Invalid number of types!\n" << ANSI_RESET;
                    delete bundle;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    break;
                }
                
                int totalPlantCount = 0;
                
                for (int i = 0; i < numTypes; i++) {
                    std::cout << "\n    " << ANSI_CYAN << "Plant " << (i+1) << " of " << numTypes << ANSI_RESET << "\n";
                    std::cout << "    " << ANSI_YELLOW << "➤ Plant number (1-" << plants.size() << "): " << ANSI_RESET;
                    int plantNum;
                    std::cin >> plantNum;
                    
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << ANSI_RED << "    ✗ Skipping invalid input...\n" << ANSI_RESET;
                        continue;
                    }
                    std::cin.ignore();
                    
                    if (plantNum < 1 || plantNum > (int)plants.size()) {
                        std::cout << ANSI_RED << "    ✗ Skipping invalid selection...\n" << ANSI_RESET;
                        continue;
                    }
                    
                    std::cout << "    " << ANSI_YELLOW << "➤ Quantity: " << ANSI_RESET;
                    int qty;
                    std::cin >> qty;
                    
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << ANSI_RED << "    ✗ Skipping invalid input...\n" << ANSI_RESET;
                        continue;
                    }
                    std::cin.ignore();
                    
                    if (qty < 1) {
                        std::cout << ANSI_RED << "    ✗ Skipping invalid quantity...\n" << ANSI_RESET;
                        continue;
                    }
                    
                    totalPlantCount += qty;
                    
                    std::string plantType = plants[plantNum-1]->getProfile()->getSpeciesName();
                    SinglePlant* bundlePlant = new SinglePlant(plantType, 25.99, qty);
                    bundle->addItem(bundlePlant);
                    
                    std::cout << ANSI_GREEN << "    ✓ Added " << qty << "x " << plantType << "\n" << ANSI_RESET;
                }
                
                if (totalPlantCount == 0) {
                    std::cout << ANSI_RED << "    ✗ No plants added to bundle!\n" << ANSI_RESET;
                    delete bundle;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    break;
                }
                
                // Calculate automatic discount based on total plant count
                OrderUIFacade* facade = customer->getUIFacade();
                double automaticDiscount = facade->calculateAutomaticDiscount(totalPlantCount);
                
                // Update bundle with automatic discount
                bundle->setDiscount(automaticDiscount);
                
                std::cout << "\n    " << ANSI_BOLD << ANSI_GREEN << "[AUTOMATIC DISCOUNT APPLIED]" << ANSI_RESET << "\n";
                std::cout << "    Total plants: " << totalPlantCount << "\n";
                std::cout << "    Discount: " << automaticDiscount << "%\n";
                
                // Create order if it doesn't exist
                if (!currentOrder) {
                    currentOrder = orderBuilder->getOrder();
                }
                
                currentOrder->addOrderItem(bundle);
                std::cout << "\n    " << ANSI_GREEN << ANSI_BOLD << "✓ Bundle '" << bundleName 
                         << "' created successfully!\n" << ANSI_RESET;
                std::this_thread::sleep_for(std::chrono::milliseconds(1200));
                break;
            }
            
            case 4: {
                // Browse bouquet suggestions (Template Method Pattern)
                browseBouquetSuggestions();
                break;
            }
            
            case 5: {
                // View current order with enhanced display
                TerminalUI::clearScreen();
                if (!currentOrder || currentOrder->isEmpty()) {
                    std::cout << "\n" << ANSI_YELLOW;
                    std::cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
                    std::cout << "    ║                     🛒 YOUR CART IS EMPTY                        ║\n";
                    std::cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
                    std::cout << ANSI_RESET << "\n";
                    std::cout << "    Start shopping by adding plants to your cart!\n";
                } else {
                    std::cout << "\n" << ANSI_CYAN << ANSI_BOLD;
                    std::cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
                    std::cout << "    ║                      🛒 YOUR CURRENT ORDER                       ║\n";
                    std::cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
                    std::cout << ANSI_RESET << "\n";
                    
                    std::string summary = currentOrder->getOrderSummary();
                    // Indent the summary
                    size_t pos = 0;
                    while ((pos = summary.find('\n', pos)) != std::string::npos) {
                        summary.insert(pos + 1, "    ");
                        pos += 5;
                    }
                    std::cout << "    " << summary << "\n";
                }
                
                std::cout << "\n    " << ANSI_CYAN << "Press Enter to continue..." << ANSI_RESET;
                std::cin.get();
                break;
            }
            
            case 6: {
                // Save order snapshot (Memento Pattern)
                std::cout << "\n    " << ANSI_YELLOW << "⚠ Order snapshot feature temporarily disabled!\n" << ANSI_RESET;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                /*
                if (!currentOrder || currentOrder->isEmpty()) {
                    std::cout << "\n    " << ANSI_YELLOW << "⚠ No order to save!\n" << ANSI_RESET;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                } else {
                    if (savedMemento) {
                        delete savedMemento;
                    }
                    savedMemento = currentOrder->createMemento();
                    std::cout << "\n    " << ANSI_GREEN << "✓ Order snapshot saved!\n" << ANSI_RESET;
                    std::cout << "    Saved " << currentOrder->getItemCount() << " items\n";
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                }
                */
                break;
            }
            
            case 7: {
                // Restore last order snapshot (Memento Pattern)
                std::cout << "\n    " << ANSI_YELLOW << "⚠ Order restore feature temporarily disabled!\n" << ANSI_RESET;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                /*
                if (!savedMemento) {
                    std::cout << "\n    " << ANSI_YELLOW << "⚠ No saved snapshot available!\n" << ANSI_RESET;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                } else {
                    if (!currentOrder) {
                        currentOrder = orderBuilder->getOrder();
                    }
                    currentOrder->restoreState(savedMemento);
                    std::cout << "\n    " << ANSI_GREEN << "✓ Order restored from snapshot!\n" << ANSI_RESET;
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                }
                */
                break;
            }
            
            case 8: {
                // Checkout with payment processing (Adapter Pattern)
                if (!currentOrder || currentOrder->isEmpty()) {
                    TerminalUI::clearScreen();
                    std::cout << "\n" << ANSI_RED;
                    std::cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
                    std::cout << "    ║                    ✗ CART IS EMPTY                               ║\n";
                    std::cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
                    std::cout << ANSI_RESET << "\n";
                    std::cout << "    Add items before checking out!\n";
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                    break;
                }
                
                TerminalUI::clearScreen();
                std::cout << "\n" << ANSI_GREEN << ANSI_BOLD;
                std::cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
                std::cout << "    ║                    💳 CHECKOUT PROCESS                           ║\n";
                std::cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
                std::cout << ANSI_RESET << "\n";
                
                // Show order summary
                std::cout << "    " << ANSI_CYAN << "Order Summary:\n" << ANSI_RESET;
                std::cout << "    Items: " << currentOrder->getItemCount() << "\n";
                std::cout << "    Total: " << ANSI_GREEN << "$" << std::fixed << std::setprecision(2) 
                         << currentOrder->getTotalAmount() << ANSI_RESET << "\n\n";
                
                // Payment method selection
                std::cout << "    " << ANSI_YELLOW << "Select Payment Method:\n" << ANSI_RESET;
                std::cout << "    " << std::string(66, '-') << "\n";
                std::cout << "    1. 💵 Cash\n";
                std::cout << "    2. 💳 Credit Card\n";
                std::cout << "    3. 🏦 EFT (Bank Transfer)\n";
                std::cout << "    " << std::string(66, '-') << "\n";
                std::cout << "\n    " << ANSI_YELLOW << "➤ Choice (1-3): " << ANSI_RESET;
                
                int paymentChoice;
                std::cin >> paymentChoice;
                
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << ANSI_RED << "    ✗ Invalid input! Please enter a number.\n" << ANSI_RESET;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    break;
                }
                std::cin.ignore();
                
                if (paymentChoice < 1 || paymentChoice > 3) {
                    std::cout << ANSI_RED << "    ✗ Invalid payment method!\n" << ANSI_RESET;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    break;
                }
                
                std::string paymentType, paymentDetails;
                
                switch(paymentChoice) {
                    case 1:
                        paymentType = "CASH";
                        paymentDetails = "CASH";
                        break;
                    case 2: {
                        paymentType = "CREDIT_CARD";
                        std::cout << "\n    " << ANSI_YELLOW << "➤ Card Number: " << ANSI_RESET;
                        std::string card;
                        std::cin >> card;
                        std::cin.ignore();
                        std::cout << "    " << ANSI_YELLOW << "➤ Expiry (MM/YY): " << ANSI_RESET;
                        std::string expiry;
                        std::cin >> expiry;
                        std::cin.ignore();
                        std::cout << "    " << ANSI_YELLOW << "➤ CVV: " << ANSI_RESET;
                        std::string cvv;
                        std::cin >> cvv;
                        std::cin.ignore();
                        paymentDetails = card + ";" + expiry + ";" + cvv;
                        break;
                    }
                    case 3:
                        paymentType = "EFT";
                        paymentDetails = "EFT";
                        break;
                }
                
                std::cout << "\n";
                showLoadingBar("Processing order", 1500);
                showLoadingBar("Validating inventory", 1000);
                
                // Create validation handler and process
                OrderValidationHandler* validator = new OrderValidationHandler();
                bool validationSuccess = validator->handleOrder(currentOrder, customer);
                
                if (!validationSuccess) {
                    std::cout << "\n" << ANSI_RED << ANSI_BOLD;
                    std::cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
                    std::cout << "    ║                  ✗ ORDER VALIDATION FAILED                       ║\n";
                    std::cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
                    std::cout << ANSI_RESET << "\n";
                    
                    std::vector<std::string> errors = validator->getValidationErrors();
                    for (size_t i = 0; i < errors.size(); i++) {
                        std::cout << "    " << (i+1) << ". " << errors[i] << "\n";
                    }
                    
                    delete validator;
                    std::cout << "\n    " << ANSI_CYAN << "Press Enter to continue..." << ANSI_RESET;
                    std::cin.get();
                    break;
                }
                
                delete validator;
                
                // Process payment using Adapter Pattern
                showLoadingBar("Processing payment", 1200);
                
                double totalAmount = currentOrder->getTotalAmount();
                bool paymentSuccess = customer->processPayment(paymentType, totalAmount, paymentDetails);
                
                if (paymentSuccess) {
                    // Update inventory - remove sold plants
                    InventoryManager& inventory = InventoryManager::getInstance();
                    
                    for (OrderItem* item : currentOrder->getOrderItems()) {
                        if (SinglePlant* plant = dynamic_cast<SinglePlant*>(item)) {
                            inventory.sellPlants(plant->getPlantType(), plant->getQuantity());
                        } else if (PlantBundle* bundle = dynamic_cast<PlantBundle*>(item)) {
                            for (OrderItem* bundleItem : bundle->getItems()) {
                                if (SinglePlant* bplant = dynamic_cast<SinglePlant*>(bundleItem)) {
                                    inventory.sellPlants(bplant->getPlantType(), bplant->getQuantity());
                                }
                            }
                        }
                    }
                    
                    currentOrder->setStatus("Completed - Paid");
                    
                    std::cout << "\n" << ANSI_GREEN << ANSI_BOLD;
                    std::cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
                    std::cout << "    ║                  ✓ ORDER SUCCESSFUL!                             ║\n";
                    std::cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
                    std::cout << ANSI_RESET << "\n";
                    std::cout << "    Thank you for your purchase!\n";
                    std::cout << "    Order ID: " << currentOrder->getOrderId() << "\n";
                    std::cout << "    Payment Method: " << paymentType << "\n";
                    std::cout << "    Amount Paid: $" << std::fixed << std::setprecision(2) << totalAmount << "\n";
                    std::cout << "    A confirmation has been sent to your email.\n";
                    
                    // Reset order for potential new purchase
                    delete currentOrder;
                    currentOrder = NULL;
                    orderBuilder->reset();
                    
                } else {
                    std::cout << "\n" << ANSI_RED << ANSI_BOLD;
                    std::cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
                    std::cout << "    ║                  ✗ PAYMENT FAILED                                ║\n";
                    std::cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
                    std::cout << ANSI_RESET << "\n";
                    std::cout << "    Payment could not be processed.\n";
                    std::cout << "    Please check your payment details and try again.\n";
                }
                
                std::cout << "\n    " << ANSI_CYAN << "Press Enter to continue..." << ANSI_RESET;
                std::cin.get();
                break;
            }
            
            case 9: {
                // Exit customer menu
                customerActive = false;
                TerminalUI::clearScreen();
                std::cout << "\n" << ANSI_GREEN << ANSI_BOLD;
                std::cout << "    ╔══════════════════════════════════════════════════════════════════╗\n";
                std::cout << "    ║          🌿 THANK YOU FOR VISITING THE GREENHOUSE! 🌿           ║\n";
                std::cout << "    ╚══════════════════════════════════════════════════════════════════╝\n";
                std::cout << ANSI_RESET << "\n";
                std::cout << "    Thank you for shopping with us, " << customer->getName() << "!\n";
                std::cout << "    Have a wonderful day!\n\n";
                break;
            }
            
            default:
                std::cout << "\n    " << ANSI_RED << "✗ Invalid choice! Try again.\n" << ANSI_RESET;
                std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    
    // ============================================================================
    // Cleanup customer resources
    // ============================================================================
    if (currentOrder) {
        delete currentOrder;
    }
    /*
    if (savedMemento) {
        delete savedMemento;
    }
    */
    delete orderBuilder;
    delete customer;
    
    TerminalUI::printSuccess("Customer interaction complete!");
    std::cout << std::endl;
}

/**
 * @brief Clean up all allocated resources
 * 
 * Proper cleanup order:
 * 1. Command prototypes (static)
 * 2. Inventory manager (owns plants)
 * 3. Plant profiles
 * 4. Staff handlers
 * 5. Staff dispatcher and manager
 * 
 * @param ctx Staff context
 * @param profiles Plant species profiles to delete
 */
void cleanup(StaffContext& ctx, std::vector<PlantSpeciesProfile*>& profiles) {
    TerminalUI::printSection("SYSTEM CLEANUP");
    
    // Clean up profiles (plants are owned by InventoryManager)
    for (size_t i = 0; i < profiles.size(); ++i) {
        delete profiles[i];
    }
    TerminalUI::printInfo("Plant profiles cleaned up");
    
    // Clean up staff handlers
    for (size_t i = 0; i < ctx.handlers.size(); ++i) {
        delete ctx.handlers[i];
    }
    TerminalUI::printInfo("Staff handlers cleaned up");
    
    // Clean up dispatcher and manager
    delete ctx.dispatcher;
    delete ctx.manager;
    TerminalUI::printInfo("Staff dispatcher and manager cleaned up");
    
    // Clean up command prototypes
    Command::cleanupPrototypes();
    TerminalUI::printInfo("Command prototypes cleaned up");
    
    // Clean up inventory (this deletes all plants)
    InventoryManager::getInstance().cleanup();
    TerminalUI::printInfo("Inventory manager cleaned up");
    
    TerminalUI::printSuccess("System cleanup complete");
}

/**
 * @brief Main entry point for the integrated greenhouse management system
 * 
 * Execution Flow:
 * ==============
 * 1. System initialization
 *    - Clear screen and display welcome header
 *    - Register command prototypes (Prototype pattern)
 *    - Create staff chains (Chain of Responsibility)
 * 
 * 2. Phase 1: Greenhouse Lifecycle Simulation
 *    - Create and populate greenhouse inventory
 *    - Simulate plant lifecycle progression
 *    - Staff handles care commands
 *    - Plants transition to ReadyForSale
 *    - Plants moved to sales floor
 * 
 * 3. Phase 2: Customer Order Simulation
 *    - Customer views available plants
 *    - Customer builds order with automatic discounts
 *    - Order validation through staff chain
 *    - Payment processing
 *    - Order confirmation
 * 
 * 4. System cleanup
 *    - Free all allocated resources
 *    - Clean inventory
 *    - Clean command prototypes
 * 
 * Design Note:
 * ===========
 * This integrated main demonstrates the complete end-to-end flow of the
 * greenhouse management system. Both phases use the same inventory instance,
 * ensuring that plants grown in the greenhouse are available for customer
 * purchase. The modular design allows each phase to be run independently
 * if needed.
 * 
 * @return 0 on successful execution
 */

/**
 * @brief Run integrated simulation with real-time GUI support (Option B)
 * 
 * This function runs the plant lifecycle simulation while simultaneously
 * listening for GUI commands on a network socket. The GUI can:
 * - Add plants in real-time
 * - Add workers in real-time
 * - Query current state
 * - Advance specific plants
 * 
 * All changes are immediately visible in both the terminal display and GUI.
 */
void runIntegratedSimulation(StaffContext& staff, GreenhouseManager* ghManager,
                            std::vector<PlantSpeciesProfile*>& profiles,
                            std::vector<PlantProduct*>& plants) {
    
    // Set greenhouse manager for MoveToSalesFloorCommand
    MoveToSalesFloorCommand::setGreenhouseManager(ghManager);
    
    // Start network server for GUI communication
    NetworkServer server;
    const int GUI_PORT = 8765;
    
    if (!server.start(GUI_PORT)) {
        TerminalUI::printError("Failed to start network server on port " + std::to_string(GUI_PORT));
        TerminalUI::printInfo("Continuing with terminal-only mode...");
    } else {
        TerminalUI::printSuccess("GUI server started on port " + std::to_string(GUI_PORT));
        TerminalUI::printInfo("Connect GUI to localhost:" + std::to_string(GUI_PORT));
    }
    
    // Simulation parameters
    const int maxSimulationSeconds = 300; // 5 minutes
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::map<std::string, std::string> stateHistory;
    int loopCounter = 0;
    
    TerminalUI::printInfo("Starting integrated simulation with GUI support...");
    std::cout << "\n" << ANSI_CYAN << "Press Ctrl+C to stop simulation" << ANSI_RESET << "\n\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    while (true) {
        // Try to accept GUI client if not connected (non-blocking)
        if (!server.isClientConnected()) {
            server.acceptClient();
        }
        
        // Check for GUI commands (non-blocking)
        std::string command;
        if (server.isClientConnected() && server.readCommand(command)) {
            std::istringstream iss(command);
            std::string cmd;
            iss >> cmd;
            
            if (cmd == "GET_PLANTS") {
                auto allPlants = InventoryManager::getInstance().getGreenHouseInventory();
                std::ostringstream response;
                response << "PLANTS " << allPlants.size() << "\n";
                for (size_t i = 0; i < allPlants.size(); i++) {
                    PlantProduct* plant = allPlants[i];
                    response << i << "|" 
                            << plant->getId() << "|"
                            << plant->getProfile()->getSpeciesName() << "|"
                            << plant->getCurrentStateName() << "|"
                            << "25.99\n";
                }
                response << "END";
                server.sendResponse(response.str());
                
            } else if (cmd == "GET_STATS") {
                auto allPlants = InventoryManager::getInstance().getGreenHouseInventory();
                int total = allPlants.size();
                int healthy = 0;
                for (auto* plant : allPlants) {
                    std::string state = plant->getCurrentStateName();
                    if (state != "Withering") healthy++;
                }
                std::ostringstream response;
                response << "STATS\n";
                response << "total_plants|" << total << "\n";
                response << "healthy_plants|" << healthy << "\n";
                response << "orders|0\n";
                response << "revenue|0.00\n";
                response << "END";
                server.sendResponse(response.str());
                
            } else if (cmd == "ADD_PLANT") {
                std::string species, type, pot;
                double price;
                iss >> species >> type >> pot >> price;
                
                // Create appropriate profile based on type with distinct durations
                PlantSpeciesProfile* profile = NULL;
                if (type == "Flower") {
                    profile = new FlowerProfile(species, "200ml", "Partial Sun", "Loamy");
                    std::cout << "[ADD_PLANT] Created Flower with 20s state duration" << std::endl;
                } else if (type == "Tree") {
                    profile = new TreeProfile(species, "300ml", "Full Sun", "Well-drained");
                    std::cout << "[ADD_PLANT] Created Tree with 30s state duration" << std::endl;
                } else if (type == "Succulent") {
                    profile = new SucculentProfile(species, "100ml", "Bright Indirect", "Sandy");
                    std::cout << "[ADD_PLANT] Created Succulent with 25s state duration" << std::endl;
                } else {
                    profile = new FlowerProfile(species, "200ml", "Partial Sun", "Loamy");
                    std::cout << "[ADD_PLANT] Created default Flower profile" << std::endl;
                }
                
                std::string plantId = "Plant_" + std::to_string(plants.size());
                PlantProduct* newPlant = new PlantProduct(plantId, profile);
                
                // CRITICAL: Attach StaffManager as observer so care commands are generated
                newPlant->setObserver(staff.manager);
                
                InventoryManager::getInstance().addToGreenhouse(newPlant);
                ghManager->addPlantToStructure(newPlant);
                profiles.push_back(profile);
                plants.push_back(newPlant);
                
                std::cout << "[ADD_PLANT] " << plantId << " (" << species << " - " << type << ") created successfully" << std::endl;
                server.sendResponse("OK " + plantId + " created successfully");
                
            } else if (cmd == "ADD_WORKER") {
                std::string name, role;
                iss >> name >> role;
                
                StaffChainHandler* worker = NULL;
                std::string teamRole;
                if (role == "Gardener" || role == "gardener") {
                    worker = new Gardener();
                    teamRole = "Greenhouse"; // Must match getRequiredRole() in care commands
                } else {
                    worker = new Cashier();
                    teamRole = "Sales";
                }
                
                worker->setManager(staff.dispatcher);
                
                // Link new worker into the chain
                if (!staff.handlers.empty()) {
                    // Find last worker of same type and link
                    for (int i = staff.handlers.size() - 1; i >= 0; i--) {
                        bool sameType = (teamRole == "Greenhouse" && dynamic_cast<Gardener*>(staff.handlers[i])) ||
                                       (teamRole == "Sales" && dynamic_cast<Cashier*>(staff.handlers[i]));
                        if (sameType) {
                            staff.handlers[i]->setNext(worker);
                            break;
                        }
                    }
                }
                
                staff.handlers.push_back(worker);
                staff.roster.push_back(std::make_pair(name, worker));
                
                // Register first worker of each team with dispatcher
                bool isFirstOfTeam = true;
                for (size_t i = 0; i < staff.handlers.size() - 1; i++) {
                    bool sameType = (teamRole == "Greenhouse" && dynamic_cast<Gardener*>(staff.handlers[i])) ||
                                   (teamRole == "Sales" && dynamic_cast<Cashier*>(staff.handlers[i]));
                    if (sameType) {
                        isFirstOfTeam = false;
                        break;
                    }
                }
                if (isFirstOfTeam) {
                    staff.dispatcher->registerTeam(teamRole, worker);
                    std::cout << "[INTEGRATED] Registered first " << teamRole << " team worker: " << name << std::endl;
                } else {
                    std::cout << "[INTEGRATED] Added " << teamRole << " worker to existing chain: " << name << std::endl;
                }
                
                server.sendResponse("OK Worker " + name + " hired as " + role);
                
            } else if (cmd == "GET_WORKERS") {
                std::ostringstream response;
                response << "WORKERS " << staff.roster.size() << "\n";
                for (size_t i = 0; i < staff.roster.size(); i++) {
                    std::string workerName = staff.roster[i].first;
                    StaffChainHandler* handler = staff.roster[i].second;
                    
                    std::string role = "Worker";
                    if (dynamic_cast<Gardener*>(handler)) {
                        role = "Gardener";
                    } else if (dynamic_cast<Cashier*>(handler)) {
                        role = "Cashier";
                    }
                    
                    response << i << "|" << workerName << "|" << role << "\n";
                }
                response << "END";
                server.sendResponse(response.str());
                
            } else if (cmd == "ADVANCE") {
                int plantId;
                iss >> plantId;
                auto allPlants = InventoryManager::getInstance().getGreenHouseInventory();
                if (plantId >= 0 && plantId < (int)allPlants.size()) {
                    allPlants[plantId]->advanceLifecycle();
                    server.sendResponse("OK Lifecycle advanced for plant " + std::to_string(plantId));
                } else {
                    server.sendResponse("ERROR Invalid plant ID");
                }
                
            } else if (cmd == "GET_INVENTORY") {
                // Get plants available for sale (on sales floor)
                auto salesFloorPlants = InventoryManager::getInstance().getReadyForSalePlants();
                std::ostringstream response;
                response << "INVENTORY " << salesFloorPlants.size() << "\n";
                for (auto* plant : salesFloorPlants) {
                    response << plant->getId() << "|"
                            << plant->getProfile()->getSpeciesName() << "|"
                            << plant->getCurrentStateName() << "|"
                            << "25.99\n";
                }
                response << "END";
                server.sendResponse(response.str());
                
            } else if (cmd == "SUGGEST_BOUQUET") {
                std::string eventType;
                iss >> eventType;
                
                BouquetSuggestionTemplate* tmpl = BouquetSuggestionFactory::getInstance().getTemplate(eventType);
                if (!tmpl) {
                    server.sendResponse("ERROR Unknown event type: " + eventType);
                } else {
                    std::vector<BouquetSuggestion> suggestions = tmpl->generateSuggestions();
                    std::ostringstream response;
                    response << "SUGGESTIONS\n";
                    response << "═══ " << eventType << " Bouquet Options ═══\n\n";
                    
                    for (const auto& suggestion : suggestions) {
                        response << "🌸 " << suggestion.bouquetName << " - $" 
                                << std::fixed << std::setprecision(2) << suggestion.estimatedPrice << "\n";
                        response << "   Flowers: ";
                        
                        for (size_t i = 0; i < suggestion.flowerTypes.size(); i++) {
                            response << suggestion.quantities[i] << "x " << suggestion.flowerTypes[i];
                            if (i < suggestion.flowerTypes.size() - 1) response << ", ";
                        }
                        
                        response << "\n   Colors: " << suggestion.colorScheme << "\n";
                        response << "   " << suggestion.significance << "\n";
                        
                        // Check inventory availability
                        response << "   Stock: ";
                        bool allAvailable = true;
                        for (size_t i = 0; i < suggestion.flowerTypes.size(); i++) {
                            int available = InventoryManager::getInstance().getAvailablePlantCount(suggestion.flowerTypes[i]);
                            int needed = suggestion.quantities[i];
                            if (available < needed) {
                                allAvailable = false;
                            }
                        }
                        if (allAvailable) {
                            response << "✓ Available\n";
                        } else {
                            response << "⚠ Limited availability\n";
                        }
                        response << "\n";
                    }
                    
                    response << "END";
                    server.sendResponse(response.str());
                }
                
            } else if (cmd == "CREATE_ORDER") {
                std::vector<std::string> plantIds;
                std::string id;
                while (iss >> id) {
                    plantIds.push_back(id);
                }
                
                if (plantIds.empty()) {
                    server.sendResponse("ERROR No plants specified");
                } else {
                    // Remove plants from sales floor inventory
                    auto salesFloorPlants = InventoryManager::getInstance().getReadyForSalePlants();
                    int soldCount = 0;
                    
                    for (const auto& plantId : plantIds) {
                        // Find plant by ID
                        for (auto* plant : salesFloorPlants) {
                            if (plant->getId() == plantId) {
                                InventoryManager::getInstance().removeFromSalesFloor(plant);
                                InventoryManager::getInstance().markAsSold(plant);
                                soldCount++;
                                std::cout << "[ORDER] Sold plant: " << plantId << std::endl;
                                break;
                            }
                        }
                    }
                    
                    double total = soldCount * 25.99;
                    std::ostringstream response;
                    response << "OK Order created successfully! Total: $" 
                            << std::fixed << std::setprecision(2) << total 
                            << " (" << soldCount << " plants sold)";
                    server.sendResponse(response.str());
                    
                    std::cout << "[ORDER] Completed order | Plants sold: " << soldCount 
                             << " | Total: $" << total << std::endl;
                }
                
            } else if (cmd == "PRINT_STRUCTURE") {
                std::cout << "\n[GUI REQUEST] Printing greenhouse structure..." << std::endl;
                ghManager->displayGreenhouseStructure();
                server.sendResponse("OK Structure printed to terminal");
                
            } else if (cmd == "EXIT") {
                server.sendResponse("OK Goodbye");
                break;
            }
        }
        
        // Advance all plant lifecycles
        auto allPlants = InventoryManager::getInstance().getGreenHouseInventory();
        for (size_t i = 0; i < allPlants.size(); ++i) {
            allPlants[i]->advanceLifecycle();
        }
        
        // Process staff command queue
        if (loopCounter % 2 == 0) {
            staff.dispatcher->processUnhandledQueue();
        }
        
        // Calculate elapsed time
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        int elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
        
        // Update state history
        for (size_t i = 0; i < allPlants.size(); ++i) {
            std::string plantId = allPlants[i]->getId();
            std::string currentState = allPlants[i]->getCurrentStateName();
            if (stateHistory.find(plantId) == stateHistory.end()) {
                stateHistory[plantId] = currentState;
            }
        }
        
        // Display live status
        TerminalUI::clearScreen();
        TerminalUI::printHeader("GREENHOUSE - INTEGRATED MODE (Option B)");
        TerminalUI::printSection("SIMULATION CLOCK");
        TerminalUI::printInfo("Elapsed: " + std::to_string(elapsed) + "s | " +
                            "Plants: " + std::to_string(allPlants.size()) + " | " +
                            "Workers: " + std::to_string(staff.roster.size()));
        TerminalUI::printInfo("GUI Server: " + std::string(server.isClientConnected() ? "CONNECTED" : "Waiting..."));
        
        displayStateTransitions(allPlants, stateHistory);
        renderPlantVisualizer(allPlants);
        displayStaffStatus(staff.roster);
        
        // Check termination
        if (elapsed >= maxSimulationSeconds) {
            TerminalUI::printWarning("Simulation time limit reached");
            break;
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
        loopCounter++;
    }
    
    server.stop();
    TerminalUI::printSuccess("Integrated simulation completed");
}

int main() {
    // ============================================================================
    // Launch Mode Selection
    // ============================================================================
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║   GREENHOUSE MANAGEMENT SYSTEM - LAUNCH MODE SELECTION     ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
    std::cout << "Select launch mode:\n";
    std::cout << "  1. Integrated Mode - C++ Backend + Java GUI (RECOMMENDED)\n";
    std::cout << "  2. Console Simulation Mode\n";
    std::cout << "\n";
    std::cout << "Enter choice (1-2): ";
    
    int modeChoice = 0;
    std::cin >> modeChoice;
    std::cin.ignore(); // Clear newline
    
    if (modeChoice == 1) {
        // ============================================================================
        // INTEGRATED MODE (RECOMMENDED)
        // Terminal simulation + Real-time GUI via network socket
        // ============================================================================
        
        std::cout << "\n";
        std::cout << "╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "║              INTEGRATED MODE                               ║\n";
        std::cout << "╠════════════════════════════════════════════════════════════╣\n";
        std::cout << "║  This mode runs the terminal simulation while listening    ║\n";
        std::cout << "║  for GUI commands on a network socket (port 8765).         ║\n";
        std::cout << "║                                                            ║\n";
        std::cout << "║  Features:                                                 ║\n";
        std::cout << "║  ✓ Real-time terminal visualization                        ║\n";
        std::cout << "║  ✓ GUI can connect and add plants/workers                  ║\n";
        std::cout << "║  ✓ Changes instantly visible in both terminal and GUI      ║\n";
        std::cout << "║  ✓ Single unified state                                    ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════╝\n";
        std::cout << "\n";
        
        // Initialize system
        registerCareCommands();
        StaffContext staff = createStaffContext();
        
        GreenhouseManager* ghManager = new GreenhouseManager();
        ghManager->buildGreenhouseStructure();
        
        // START EMPTY - user adds everything via GUI
        std::vector<PlantSpeciesProfile*> profiles;
        std::vector<PlantProduct*> plants;
        
        std::cout << "Press Enter to start integrated simulation...";
        std::cin.get();
        
        // Run integrated simulation
        runIntegratedSimulation(staff, ghManager, profiles, plants);
        
        // Cleanup
        delete ghManager;
        cleanup(staff, profiles);
        
        return 0;
    }
    
    // ============================================================================
    // CONSOLE MODE - Original Simulation
    // ============================================================================
    // ============================================================================
    // System Initialization
    // ============================================================================
    TerminalUI::clearScreen();
    TerminalUI::printHeader("INTEGRATED GREENHOUSE MANAGEMENT SYSTEM");
    TerminalUI::printInfo("Initializing system components...");
    std::cout << std::endl;

    TerminalUI::printSection("SYSTEM INITIALISATION");
    
    // Register command prototypes
    registerCareCommands();

    // Create staff context (dispatcher, manager, chains)
    StaffContext staff = createStaffContext();
    
    // Store profiles for cleanup
    std::vector<PlantSpeciesProfile*> profiles;
    
    std::cout << std::endl;
    TerminalUI::printSuccess("System initialization complete!");
    std::cout << std::endl;
    
    // Add a brief pause to let user see initialization
    std::cout << "Press Enter to start greenhouse simulation...";
    std::cin.get();
    std::cout << std::endl;

    // ============================================================================
    // Phase 1: Greenhouse Lifecycle Simulation
    // ============================================================================
    runGreenhouseSimulation(staff);
    
    // Add pause between phases
    std::cout << "Press Enter to start customer order simulation...";
    std::cin.get();
    std::cout << std::endl;

    // ============================================================================
    // Phase 2: Customer Order Simulation
    // ============================================================================
    runCustomerOrderTest(staff);

    // ============================================================================
    // System Cleanup
    // ============================================================================
    std::cout << std::endl;
    profiles = createProfiles();  // Get profiles for cleanup
    cleanup(staff, profiles);
    
    std::cout << std::endl;
    TerminalUI::printSuccess("Program execution complete. Goodbye!");
    std::cout << std::endl;

    return 0;
}
