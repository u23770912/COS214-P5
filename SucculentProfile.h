#ifndef SUCCULENT_PROFILE_H
#define SUCCULENT_PROFILE_H

#include <string>
#include "PlantSpeciesProfile.h"

class SucculentProfile : public PlantSpeciesProfile {
public:
	SucculentProfile(const std::string &species,
					 const std::string &idealWater,
					 const std::string &sunlight,
					 const std::string &soil);
};

#endif // SUCCULENT_PROFILE_H
