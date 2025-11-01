#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "CareCommand.h"
#include "Cashier.h"
#include "FertilizeCommand.h"
#include "FlowerProfile.h"
#include "Gardener.h"
#include "MoveToSalesFloorCommand.h"
#include "PlantProduct.h"
#include "PruneCommand.h"
#include "StaffChainHandler.h"
#include "StaffManager.h"
#include "StaffMember.h"
#include "SucculentProfile.h"
#include "TerminalUI.h"
#include "TreeProfile.h"
#include "WaterCommand.h"
#include "InventoryManager.h"
#include "PotFactory.h"
#include "ClayPotFactory.h"
#include "GlassPotFactory.h"
#include "MetalPotFactory.h"
#include "PlasticPotFactory.h"
#include "WoodenPotFactory.h"

namespace
{

    struct StaffContext
    {
        StaffMember *dispatcher = nullptr;
        StaffManager *manager = nullptr;
        std::vector<StaffChainHandler *> handlers;
        std::vector<std::pair<std::string, StaffChainHandler *>> roster;
    };

    void registerCareCommands()
    {
        static bool registered = false;
        if (registered)
        {
            return;
        }

        CareCommand::registerCommand("Watering", new WaterCommand());
        CareCommand::registerCommand("Pruning", new PruneCommand());
        CareCommand::registerCommand("Fertilizing", new FertilizeCommand());
        CareCommand::registerCommand("MoveToSalesFloor", new MoveToSalesFloorCommand());
        registered = true;
    }

    StaffContext createStaffContext()
    {
        StaffContext ctx;

        // Create 5 Gardeners
        Gardener *gardener1 = new Gardener();
        Gardener *gardener2 = new Gardener();
        Gardener *gardener3 = new Gardener();
        Gardener *gardener4 = new Gardener();
        Gardener *gardener5 = new Gardener();
        gardener1->setNext(gardener2);
        gardener2->setNext(gardener3);
        gardener3->setNext(gardener4);
        gardener4->setNext(gardener5);

        // Create 3 Cashiers
        Cashier *cashier1 = new Cashier();
        Cashier *cashier2 = new Cashier();
        Cashier *cashier3 = new Cashier();
        cashier1->setNext(cashier2);
        cashier2->setNext(cashier3);

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

        ctx.dispatcher = new StaffMember();
        ctx.dispatcher->registerTeam("Greenhouse", gardener1);
        ctx.dispatcher->registerTeam("Sales", cashier1);

        ctx.manager = new StaffManager(ctx.dispatcher);

        // CRITICAL FIX: Set manager back-reference for ALL handlers
        for (auto handler : ctx.handlers)
        {
            handler->setManager(ctx.dispatcher);
        }

        return ctx;
    }

    std::vector<PlantSpeciesProfile *> createProfiles()
    {
        std::vector<PlantSpeciesProfile *> profiles;
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

    std::vector<PlantProduct *> createPlants(const std::vector<PlantSpeciesProfile *> &profiles,
                                             StaffManager *manager)
    {
        std::vector<PlantProduct *> plants;
        int counter = 1;
        for (const auto &profile : profiles)
        {
            std::ostringstream id;
            id << "P" << std::setfill('0') << std::setw(3) << counter++;
            PlantProduct *plant = new PlantProduct(id.str(), profile);
            plant->setObserver(manager);
            plants.push_back(plant);
        }
        return plants;
    }

    void displayStateTransitions(const std::vector<PlantProduct *> &plants,
                                 std::map<std::string, std::string> &history)
    {
        TerminalUI::printSection("STATE TRANSITIONS");
        bool transitionLogged = false;
        for (const auto &plant : plants)
        {
            const std::string &currentState = plant->getCurrentStateName();
            std::string plantId = plant->getId();
            if (history[plantId] != currentState)
            {
                if (!history[plantId].empty())
                {
                    TerminalUI::printSuccess(plantId + " " + plant->getProfile()->getSpeciesName() + " [" +
                                             history[plantId] + " -> " + currentState + "]");
                }
                history[plantId] = currentState;
                transitionLogged = true;
            }
        }
        if (!transitionLogged)
        {
            TerminalUI::printInfo("No state changes detected");
        }
        TerminalUI::printDivider();
    }

    int desiredStateDuration(const PlantProduct *plant, const std::string &state)
    {
        if (state == "ReadyForSale" || state == "Withering")
        {
            return 0;
        }
        const PlantSpeciesProfile *profile = plant->getProfile();
        return profile ? profile->getStateDurationSeconds(state, 0) : 0;
    }

    void renderPlantVisualizer(const std::vector<PlantProduct *> &plants)
    {
        TerminalUI::printSection("PLANT VISUALIZER");
        const int barWidth = 24;
        for (const auto &plant : plants)
        {
            std::string state = plant->getCurrentStateName();
            int secondsInState = plant->getSecondsInCurrentState();
            int targetSeconds = desiredStateDuration(plant, state);
            int filled = (targetSeconds > 0)
                             ? std::min(barWidth, (secondsInState * barWidth) / targetSeconds)
                             : barWidth;
            std::string bar(barWidth, '.');
            for (int i = 0; i < filled; ++i)
            {
                bar[i] = '#';
            }
            std::cout << "[" << plant->getId() << "] "
                      << std::setw(12) << std::left << plant->getProfile()->getSpeciesName()
                      << " " << std::setw(14) << std::left << state
                      << " " << bar << " " << secondsInState << "s" << std::endl;
        }
        TerminalUI::printDivider();
    }

    void displayStaffStatus(const std::vector<std::pair<std::string, StaffChainHandler *>> &roster)
    {
        TerminalUI::printSection("WORKFORCE STATUS");
        for (const auto &member : roster)
        {
            const std::string &label = member.first;
            StaffChainHandler *handler = member.second;
            if (handler->isBusy())
            {
                TerminalUI::printBusy(label);
                const PlantProduct *plant = handler->getActivePlant();
                std::string task = handler->getActiveTask();
                if (plant)
                {
                    std::string strategyName = plant->getStrategyNameForCareType(task);
                    TerminalUI::printStaffAssignment(plant->getId(), task, strategyName);
                }
            }
            else
            {
                TerminalUI::printAvailable(label);
            }
        }
        TerminalUI::printDivider();
    }

    bool allPlantsReady(const std::vector<PlantProduct *> &plants)
    {
        return std::all_of(plants.begin(), plants.end(), [](const PlantProduct *plant)
                           { return plant->getCurrentStateName() == "ReadyForSale"; });
    }

    void cleanup(StaffContext &ctx,
                 std::vector<PlantProduct *> &plants,
                 std::vector<PlantSpeciesProfile *> &profiles)
    {
        // The InventoryManager now owns the plants, so we don't delete them here.
        // for (auto p : plants) delete p;
        for (auto p : profiles)
            delete p;
        for (auto h : ctx.handlers)
            delete h;
        delete ctx.dispatcher;
        delete ctx.manager;
        CareCommand::cleanupPrototypes();          // Clean up command prototypes
        InventoryManager::getInstance().cleanup(); // Clean up inventory
    }

} // namespace

void demonstratePotFactory()
{
    TerminalUI::printSection("POT FACTORY DEMONSTRATION");

    std::vector<std::pair<std::string, PotFactory *>> factories;
    factories.push_back(std::make_pair("Clay", new ClayPotFactory()));
    factories.push_back(std::make_pair("Glass", new GlassPotFactory()));
    factories.push_back(std::make_pair("Metal", new MetalPotFactory()));
    factories.push_back(std::make_pair("Plastic", new PlasticPotFactory()));
    factories.push_back(std::make_pair("Wooden", new WoodenPotFactory()));

    std::vector<std::string> sizes;
    sizes.push_back("Small");
    sizes.push_back("Medium");
    sizes.push_back("Large");

    std::vector<std::string> shapes;
    shapes.push_back("Round");
    shapes.push_back("Square");
    shapes.push_back("Rectangle");

    for (size_t f = 0; f < factories.size(); f++)
    {
        std::string name = factories[f].first;
        PotFactory *factory = factories[f].second;

        std::cout << "\n--- " << name << " Pots ---" << std::endl;

        for (int i = 0; i < 3; i++)
        {
            Pot *pot = factory->createPot(
                sizes[i % sizes.size()],
                shapes[i % shapes.size()],
                i % 2 == 0);
            pot->print();
            InventoryManager::getInstance().addPot(pot);
        }
    }

    for (size_t f = 0; f < factories.size(); f++)
    {
        delete factories[f].second;
    }

    TerminalUI::printSuccess("Created 15 different pots (3 of each type)");
    TerminalUI::printDivider();
}

int main()
{
    TerminalUI::clearScreen();
    TerminalUI::printHeader("GREENHOUSE MANAGEMENT SYSTEM");

    TerminalUI::printSection("SYSTEM INITIALISATION");
    registerCareCommands();
    TerminalUI::printSuccess("Care command prototypes registered");

    // THIS IS THE NEW CODE TO DEMONSTRATE THE POT FACTORY
    demonstratePotFactory();

    StaffContext staff = createStaffContext();
    TerminalUI::printSuccess("Staff teams configured (5 Gardeners, 3 Cashiers)");

    std::vector<PlantSpeciesProfile *> profiles = createProfiles();
    std::vector<PlantProduct *> plants = createPlants(profiles, staff.manager);
    TerminalUI::printSuccess(std::to_string(plants.size()) + " plants created");

    // Add plants to the inventory manager
    for (auto *plant : plants)
    {
        InventoryManager::getInstance().addToGreenhouse(plant);
    }

    const int maxSimulationSeconds = 120;
    auto start = std::chrono::steady_clock::now();
    std::map<std::string, std::string> stateHistory;
    for (const auto &plant : plants)
    {
        stateHistory[plant->getId()] = plant->getCurrentStateName();
    }

    int loopCounter = 0;

    while (true)
    {
        // Advance plant lifecycles
        for (auto &plant : plants)
        {
            plant->advanceLifecycle();
        }

        // CRITICAL: Process unhandled queue every 2 seconds
        // This attempts to re-dispatch queued commands when staff becomes available
        if (loopCounter % 2 == 0)
        {
            staff.dispatcher->processUnhandledQueue();
        }

        auto now = std::chrono::steady_clock::now();
        int elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

        TerminalUI::clearScreen();
        TerminalUI::printHeader("GREENHOUSE MANAGEMENT SYSTEM - LIVE VIEW");
        TerminalUI::printSection("SIMULATION CLOCK");
        TerminalUI::printInfo("Elapsed: " + std::to_string(elapsed) + "s (limit " +
                              std::to_string(maxSimulationSeconds) + "s)");

        displayStateTransitions(plants, stateHistory);
        renderPlantVisualizer(plants);
        displayStaffStatus(staff.roster);

        if (elapsed >= maxSimulationSeconds || allPlantsReady(plants))
        {
            TerminalUI::printWarning("Simulation stopping...");
            break;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
        loopCounter++;
    }

    TerminalUI::printSection("FINAL SUMMARY");
    for (const auto &plant : plants)
    {
        std::cout << "[" << plant->getId() << "] "
                  << std::setw(14) << std::left << plant->getProfile()->getSpeciesName()
                  << " -> " << plant->getCurrentStateName() << std::endl;
    }
    InventoryManager::getInstance().printInventoryReport();

    cleanup(staff, plants, profiles);
    TerminalUI::printSuccess("System shutdown complete");
    return 0;
}
