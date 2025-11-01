#include "SucculentProfile.h"

SucculentProfile::SucculentProfile(const std::string &species,
								   const std::string &idealWater,
								   const std::string &sunlight,
								   const std::string &soil)
{
	speciesName = species;
	setProperty("category", "Succulent");
	setProperty("idealWater", sanitizeNumericString(idealWater, "150"));
	setProperty("idealFertilizer", "25");
	setProperty("idealPruning", "minimal");
	setProperty("idealSunlight", sunlight.empty() ? "Bright Indirect" : sunlight);
	setProperty("idealSoil", soil.empty() ? "Sandy" : soil);
	setProperty("maxDaysWithoutCare", "10");
	setProperty("supportedCareTypes", "water,fertilize,drip,pruning");

	setStateDurationSeconds("Planted", 25);	  // You can change the duration for the Planted state here
	setStateDurationSeconds("InNursery", 25); // You can change the duration for the InNursery state here
	setStateDurationSeconds("Growing", 25);	  // You can change the duration for the Growing state here

	setCareIntervalSeconds("Watering", 12);	   // You can change the interval for Watering here
	setCareIntervalSeconds("Fertilizing", 20); // You can change the interval for Fertilizing here
	setCareIntervalSeconds("Pruning", 15);	   // You can change the interval for Pruning here
}