#include "Command.h"

// Definition and initialization of the static map from the Command base class.
std::map<std::string, Command*> Command::prototypes;

Command::~Command() {}

void Command::setReceiver(PlantProduct* /*plant*/) {}

PlantProduct* Command::getReceiver() const {
	return nullptr;
}
