#ifndef FLOWER_PROFILE_H
#define FLOWER_PROFILE_H

#include <string>
#include "PlantSpeciesProfile.h"

class FlowerProfile : public PlantSpeciesProfile {
public:
	FlowerProfile(const std::string &species,
				  const std::string &idealWater,
				  const std::string &sunlight,
				  const std::string &soil);
};

#endif // FLOWER_PROFILE_H
