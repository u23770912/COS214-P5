#ifndef TREE_PROFILE_H
#define TREE_PROFILE_H

#include <string>
#include "PlantSpeciesProfile.h"

class TreeProfile : public PlantSpeciesProfile {
public:
	TreeProfile(const std::string &species,
				const std::string &idealWater,
				const std::string &sunlight,
				const std::string &soil);
};

#endif // TREE_PROFILE_H
