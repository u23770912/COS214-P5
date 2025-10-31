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
#include <chrono>
#include <iomanip>
#include <iostream>
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

// Customer Order Infrastructure
#include "Customer.h"
#include "Order.h"
#include "ConcreteOrderBuilder.h"
#include "SinglePlant.h"
#include "PlantBundle.h"
#include "OrderValidationHandler.h"
#include "PaymentProcessHandler.h"
#include "NotificationHandler.h"

// UI Infrastructure
#include "TerminalUI.h"

// Forward declarations
struct StaffContext;
void runGreenhouseSimulation(StaffContext& staff);
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
    // Create Greenhouse Team (5 Gardeners)
    // ============================================================================
    Gardener* gardener1 = new Gardener();
    Gardener* gardener2 = new Gardener();
    Gardener* gardener3 = new Gardener();
    Gardener* gardener4 = new Gardener();
    Gardener* gardener5 = new Gardener();
    
    // Chain the gardeners together
    gardener1->setNext(gardener2);
    gardener2->setNext(gardener3);
    gardener3->setNext(gardener4);
    gardener4->setNext(gardener5);

    // ============================================================================
    // Create Sales Team (3 Cashiers)
    // ============================================================================
    Cashier* cashier1 = new Cashier();
    Cashier* cashier2 = new Cashier();
    Cashier* cashier3 = new Cashier();
    
    // Chain the cashiers together
    cashier1->setNext(cashier2);
    cashier2->setNext(cashier3);

    // ============================================================================
    // Store all handlers for cleanup and roster display
    // ============================================================================
    ctx.handlers.push_back(gardener1);
    ctx.handlers.push_back(gardener2);
    ctx.handlers.push_back(gardener3);
    ctx.handlers.push_back(gardener4);
    ctx.handlers.push_back(gardener5);
    ctx.handlers.push_back(cashier1);
    ctx.handlers.push_back(cashier2);
    ctx.handlers.push_back(cashier3);

    ctx.roster.push_back(std::make_pair(std::string("Gardener 1"), gardener1));
    ctx.roster.push_back(std::make_pair(std::string("Gardener 2"), gardener2));
    ctx.roster.push_back(std::make_pair(std::string("Gardener 3"), gardener3));
    ctx.roster.push_back(std::make_pair(std::string("Gardener 4"), gardener4));
    ctx.roster.push_back(std::make_pair(std::string("Gardener 5"), gardener5));
    ctx.roster.push_back(std::make_pair(std::string("Cashier 1"), cashier1));
    ctx.roster.push_back(std::make_pair(std::string("Cashier 2"), cashier2));
    ctx.roster.push_back(std::make_pair(std::string("Cashier 3"), cashier3));

    // ============================================================================
    // Create dispatcher and register teams
    // ============================================================================
    ctx.dispatcher = new StaffMember();
    ctx.dispatcher->registerTeam("Greenhouse", gardener1);  // Head of greenhouse chain
    ctx.dispatcher->registerTeam("Sales", cashier1);        // Head of sales chain

    // ============================================================================
    // Create staff manager (Observer for plants and customers)
    // ============================================================================
    ctx.manager = new StaffManager(ctx.dispatcher);

    // ============================================================================
    // CRITICAL: Set manager back-reference for ALL handlers
    // This allows handlers to queue commands when entire chain is busy
    // ============================================================================
    for (size_t i = 0; i < ctx.handlers.size(); ++i) {
        ctx.handlers[i]->setManager(ctx.dispatcher);
    }

    TerminalUI::printSuccess("Staff teams configured (5 Gardeners, 3 Cashiers)");
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
    TerminalUI::printInfo("Initializing greenhouse with plant inventory...");
    
    // ============================================================================
    // Phase 1.1: Create plants and add to greenhouse
    // ============================================================================
    std::vector<PlantSpeciesProfile*> profiles = createProfiles();
    std::vector<PlantProduct*> plants = createPlants(profiles, staff.manager);
    
    TerminalUI::printSuccess(std::to_string(plants.size()) + " plants created");

    // Add plants to the inventory manager (Greenhouse)
    for (size_t i = 0; i < plants.size(); ++i) {
        InventoryManager::getInstance().addToGreenhouse(plants[i]);
    }
    
    TerminalUI::printSuccess("All plants added to greenhouse inventory");
    TerminalUI::printInfo("Greenhouse inventory count: " + 
                         std::to_string(InventoryManager::getInstance().getGreenHouseInventory().size()));

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
    
    // Keep profile pointers for cleanup later
    // Note: plants are now owned by InventoryManager, profiles need manual cleanup
}

/**
 * @brief Run the customer order interaction test
 * 
 * This function simulates a customer placing an order, demonstrating:
 * 1. Viewing available plants from the sales floor
 * 2. Building an order with individual plants and bundles
 * 3. Automatic discount calculation
 * 4. Order validation through the staff chain
 * 5. Payment processing
 * 6. Order confirmation
 * 
 * The test uses the same inventory populated by the greenhouse simulation,
 * ensuring end-to-end integration.
 * 
 * @param staff Staff context with configured teams
 */
void runCustomerOrderTest(StaffContext& staff) {
    TerminalUI::printHeader("PHASE 2: CUSTOMER ORDER SIMULATION");
    TerminalUI::printInfo("Initializing customer order system...");
    std::cout << std::endl;
    
    // ============================================================================
    // Phase 2.1: Display available inventory
    // ============================================================================
    TerminalUI::printSection("SALES FLOOR INVENTORY STATUS");
    int availablePlants = InventoryManager::getInstance().getReadyForSalePlants().size();
    TerminalUI::printInfo("Plants available for purchase: " + std::to_string(availablePlants));
    
    if (availablePlants == 0) {
        TerminalUI::printWarning("No plants available on sales floor!");
        TerminalUI::printInfo("Skipping customer order test.");
        return;
    }
    
    std::cout << std::endl;
    
    // ============================================================================
    // Phase 2.2: Create customer
    // ============================================================================
    TerminalUI::printSection("CUSTOMER CREATION");
    Customer* customer = new Customer("Test Customer", "test@greenhouse.com", "555-0100");
    
    // Attach staff manager as observer to customer
    customer->attachObserver(staff.manager);
    
    TerminalUI::printSuccess("Customer created: " + customer->getName());
    TerminalUI::printInfo("Email: " + customer->getEmail());
    TerminalUI::printInfo("Phone: " + customer->getCellPhone());
    std::cout << std::endl;
    
    // ============================================================================
    // Phase 2.3: Customer views available plants
    // ============================================================================
    TerminalUI::printSection("STEP 1: VIEW AVAILABLE PLANTS");
    customer->displayAvailableItems();
    std::cout << std::endl;
    
    // ============================================================================
    // Phase 2.4: Create order builder
    // ============================================================================
    TerminalUI::printSection("STEP 2: BUILD ORDER");
    ConcreteOrderBuilder* orderBuilder = new ConcreteOrderBuilder(customer->getName());
    Order* currentOrder = orderBuilder->getOrder();
    
    // ============================================================================
    // Phase 2.5: Add plants to order
    // ============================================================================
    std::vector<PlantProduct*> salesPlants = InventoryManager::getInstance().getReadyForSalePlants();
    
    // Add first plant (quantity 2)
    if (salesPlants.size() > 0) {
        std::string plantType = salesPlants[0]->getProfile()->getSpeciesName();
        SinglePlant* plant1 = new SinglePlant(plantType, 25.99, 2, "Medium");
        currentOrder->addOrderItem(plant1);
        TerminalUI::printSuccess("Added 2x " + plantType + " to order");
    }
    
    // Add second plant (quantity 3)
    if (salesPlants.size() > 1) {
        std::string plantType = salesPlants[1]->getProfile()->getSpeciesName();
        SinglePlant* plant2 = new SinglePlant(plantType, 29.99, 3, "Large");
        currentOrder->addOrderItem(plant2);
        TerminalUI::printSuccess("Added 3x " + plantType + " to order");
    }
    
    // ============================================================================
    // Phase 2.6: Create a bundle with automatic discount
    // ============================================================================
    if (salesPlants.size() >= 3) {
        TerminalUI::printInfo("Creating plant bundle...");
        PlantBundle* bundle = new PlantBundle("Garden Starter Bundle", "Custom", 1, 0.0);
        
        // Add plants to bundle
        for (size_t i = 2; i < std::min(size_t(5), salesPlants.size()); ++i) {
            std::string plantType = salesPlants[i]->getProfile()->getSpeciesName();
            SinglePlant* bundlePlant = new SinglePlant(plantType, 25.99, 1, "Medium");
            bundle->addItem(bundlePlant);
        }
        
        // Calculate automatic discount (5 plants total in order = 5% discount)
        bundle->setDiscount(5.0);
        currentOrder->addOrderItem(bundle);
        TerminalUI::printSuccess("Added 'Garden Starter Bundle' with 5% automatic discount");
    }
    
    std::cout << std::endl;
    
    // ============================================================================
    // Phase 2.7: View current order
    // ============================================================================
    TerminalUI::printSection("STEP 3: REVIEW ORDER");
    std::cout << currentOrder->getOrderSummary() << std::endl;
    
    // ============================================================================
    // Phase 2.8: Submit order for validation
    // ============================================================================
    TerminalUI::printSection("STEP 4: ORDER VALIDATION & PROCESSING");
    TerminalUI::printInfo("Submitting order through validation chain...");
    std::cout << std::endl;
    
    // Create handler chain
    OrderValidationHandler* validator = new OrderValidationHandler();
    PaymentProcessHandler* paymentProcessor = new PaymentProcessHandler();
    NotificationHandler* successNotifier = new NotificationHandler(false);
    NotificationHandler* failureNotifier = new NotificationHandler(true);
    
    // Set up success chain: Validation -> Payment -> Success Notification
    validator->setNext(paymentProcessor);
    paymentProcessor->setNext(successNotifier);
    
    TerminalUI::printInfo("Order processing chain configured:");
    std::cout << "  1. Inventory Validation" << std::endl;
    std::cout << "  2. Payment Processing" << std::endl;
    std::cout << "  3. Customer Notification" << std::endl;
    std::cout << std::endl;
    
    // Start the chain
    TerminalUI::printInfo("Starting order processing...");
    std::cout << std::endl;
    
    bool processingResult = validator->handleOrder(currentOrder, customer);
    
    std::cout << std::endl;
    
    // ============================================================================
    // Phase 2.9: Display order result
    // ============================================================================
    if (!processingResult) {
        TerminalUI::printError("ORDER PROCESSING FAILED!");
        TerminalUI::printDivider('=');
        
        // Get validation errors if validation failed
        std::vector<std::string> errors = validator->getValidationErrors();
        if (!errors.empty()) {
            TerminalUI::printWarning("Validation errors:");
            for (size_t i = 0; i < errors.size(); ++i) {
                std::cout << "  - " << errors[i] << std::endl;
            }
            failureNotifier->setErrorMessages(errors);
        }
        
        failureNotifier->handleOrder(currentOrder, customer);
        TerminalUI::printInfo("A failure notification has been sent to " + customer->getEmail());
    } else {
        TerminalUI::printSuccess("ORDER PROCESSING COMPLETED SUCCESSFULLY!");
        TerminalUI::printDivider('=');
        TerminalUI::printInfo("Order Status: " + currentOrder->getStatus());
        TerminalUI::printInfo("Total Amount: $" + std::to_string(currentOrder->getTotalAmount()));
        TerminalUI::printInfo("Confirmation email sent to " + customer->getEmail());
    }
    
    std::cout << std::endl;
    
    // ============================================================================
    // Cleanup
    // ============================================================================
    delete validator;
    delete paymentProcessor;
    delete successNotifier;
    delete failureNotifier;
    delete currentOrder;
    delete orderBuilder;
    delete customer;
    
    TerminalUI::printSuccess("Customer order test complete!");
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
int main() {
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
