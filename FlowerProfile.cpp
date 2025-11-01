#include "FlowerProfile.h"

FlowerProfile::FlowerProfile(const std::string &species,
							 const std::string &idealWater,
							 const std::string &sunlight,
							 const std::string &soil) {
	speciesName = species;
	setProperty("category", "Flower");
	setProperty("idealWater", sanitizeNumericString(idealWater, "200"));
	setProperty("idealFertilizer", "50");
	setProperty("idealPruning", "standard");
	setProperty("idealSunlight", sunlight.empty() ? "Partial Sun" : sunlight);
	setProperty("idealSoil", soil.empty() ? "Loamy" : soil);
	setProperty("maxDaysWithoutCare", "5");
	setProperty("supportedCareTypes", "water,mist,fertilize,prune_standard");

	setStateDurationSeconds("Planted", 20);
	setStateDurationSeconds("InNursery", 20);
	setStateDurationSeconds("Growing", 20);

	setCareIntervalSeconds("Watering", 6);
	setCareIntervalSeconds("Mist", 8);
	setCareIntervalSeconds("Fertilizing", 15);
	setCareIntervalSeconds("Pruning", 18);
}