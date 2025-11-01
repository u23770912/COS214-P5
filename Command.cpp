#include "Command.h"

// Definition and initialization of the static map from the Command base class.
std::map<std::string, Command*> Command::prototypes;

Command::Command() : plantReceiver(nullptr) {}

Command::~Command() {}

void Command::registerCommand(const std::string& type, Command* prototype) {
    prototypes[type] = prototype;
}

Command* Command::createCommand(const std::string& type) {
    auto it = prototypes.find(type);
    if (it != prototypes.end()) {
        return it->second->clone();
    }
    std::cout << "Error: Command type '" << type << "' not registered." << std::endl;
    return nullptr;
}

void Command::setReceiver(PlantProduct* plant) {
    this->plantReceiver = plant;
}

PlantProduct* Command::getReceiver() const {
	return plantReceiver;
}

void Command::cleanupPrototypes() {
    for (auto const& pair : prototypes) {
        delete pair.second;
    }
    prototypes.clear();
}
