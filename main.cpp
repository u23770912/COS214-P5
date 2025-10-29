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
             << state << " "
             << bar << " "
             << secondsInState << "s";
        std::cout << line.str() << std::endl;
    }
    TerminalUI::printDivider();
}

bool allPlantsSettled(const std::vector<PlantProduct *> &plants) {
    return std::all_of(plants.begin(), plants.end(), [](PlantProduct *plant) {
        std::string state = plant->getCurrentStateName();
        return state == "ReadyForSale" || state == "Withering";
    });
}

} // namespace

int main() {
    TerminalUI::clearScreen();
    TerminalUI::printHeader("GREENHOUSE MANAGEMENT SYSTEM");

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

    TerminalUI::printInfo("Spawning plant specimens...");
    std::vector<PlantProduct *> plants;
    std::vector<PlantSpeciesProfile *> profiles;

    profiles.push_back(new MapProfile("Rose", {{"idealWater", "250"}, {"idealFertilizer", "60"}, {"idealPruning", "1"}, {"maxDaysWithoutCare", "5"}}));
    profiles.push_back(new MapProfile("Cactus", {{"idealWater", "100"}, {"idealFertilizer", "25"}, {"idealPruning", "0"}, {"maxDaysWithoutCare", "10"}}));
    profiles.push_back(new MapProfile("Bonsai", {{"idealWater", "180"}, {"idealFertilizer", "45"}, {"idealPruning", "2"}, {"maxDaysWithoutCare", "4"}}));

    int plantCounter = 1;
    for (auto *profile : profiles) {
        std::ostringstream id;
        id << "P" << std::setfill('0') << std::setw(3) << plantCounter++;
        auto *plant = new PlantProduct(id.str(), profile);
        plant->setObserver(manager);
        plants.push_back(plant);
        TerminalUI::printSuccess("Plant '" + plant->getId() + "' (" + profile->getSpeciesName() + ") ready");
    }

    TerminalUI::printDivider();

    const int maxSimulationSeconds = 240;
    auto simulationStart = std::chrono::steady_clock::now();

    while (true) {
        for (auto *plant : plants) {
            plant->advanceLifecycle();
        }

        auto now = std::chrono::steady_clock::now();
        int elapsedSeconds = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(now - simulationStart).count());

        TerminalUI::clearScreen();
        TerminalUI::printHeader("GREENHOUSE MANAGEMENT SYSTEM");
        TerminalUI::printSection("SIMULATION CLOCK");
        TerminalUI::printInfo("Elapsed: " + std::to_string(elapsedSeconds) + "s (limit " + std::to_string(maxSimulationSeconds) + "s)");

        renderPlantVisualizer(plants);
        displayStaffStatus(staff);

        if (elapsedSeconds >= maxSimulationSeconds || allPlantsSettled(plants)) {
            TerminalUI::printWarning("Simulation stopping...");
            break;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    TerminalUI::printSection("SIMULATION SUMMARY");
    for (auto *plant : plants) {
        TerminalUI::printInfo("Plant " + plant->getId() + " final state: " + plant->getCurrentStateName());
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