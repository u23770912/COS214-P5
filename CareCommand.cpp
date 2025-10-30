#include "CareCommand.h"

// Definition and initialization of the static map.
std::map<std::string, CareCommand*> CareCommand::prototypes;

void CareCommand::cleanupPrototypes() {
    for (auto const& pair : prototypes) {
        delete pair.second;
    }
    prototypes.clear();
}