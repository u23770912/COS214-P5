#include "Command.h"

// Definition and initialization of the static map from the Command base class.
std::map<std::string, Command*> Command::prototypes;

void Command::setReceiver(PlantProduct* plant) {
    // Default implementation does nothing
    // Subclasses should override this if they need to set a receiver
}
