#include "TreeProfile.h"

TreeProfile::TreeProfile(const std::string &species,
						 const std::string &idealWater,
						 const std::string &sunlight,
						 const std::string &soil) {
	speciesName = species;
	setProperty("category", "Tree");
	setProperty("idealWater", sanitizeNumericString(idealWater, "300"));
	setProperty("idealFertilizer", "80");
	setProperty("idealPruning", "artistic");
	setProperty("idealSunlight", sunlight.empty() ? "Full Sun" : sunlight);
	setProperty("idealSoil", soil.empty() ? "Well-drained" : soil);
	setProperty("maxDaysWithoutCare", "7");
	setProperty("supportedCareTypes", "water,fertilize,prune_artistic,prune_standard,drip");

	setStateDurationSeconds("Planted", 30);
	setStateDurationSeconds("InNursery", 30);
	setStateDurationSeconds("Growing", 30);

	setCareIntervalSeconds("Watering", 10);
	setCareIntervalSeconds("Fertilizing", 18);
	setCareIntervalSeconds("Pruning", 16);
}