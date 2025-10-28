#include "Command.h"

// Definition and initialization of the static map from the Command base class.
std::map<std::string, Command*> Command::prototypes;
<<<<<<< HEAD
=======

Command::~Command() {}

void Command::setReceiver(PlantProduct* /*plant*/) {}

PlantProduct* Command::getReceiver() const {
	return nullptr;
}
>>>>>>> a5cc157f06bf6b3c50e503d2c0883ab9968a4990
