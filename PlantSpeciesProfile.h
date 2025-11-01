#ifndef PLANT_SPECIES_PROFILE_H
#define PLANT_SPECIES_PROFILE_H

#include <algorithm>
#include <cctype>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class PlantSpeciesProfile {
protected:
	std::string speciesName;
	std::map<std::string, std::string> properties;

public:
	virtual ~PlantSpeciesProfile() {}

	virtual std::string getSpeciesName() const { return speciesName; }

	virtual std::string getProperty(const std::string &key) const {
		std::map<std::string, std::string>::const_iterator it = properties.find(key);
		return (it != properties.end()) ? it->second : std::string();
	}

	void setProperty(const std::string &key, const std::string &value) {
		properties[key] = value;
	}

	std::vector<std::string> getSupportedCareTypes() const {
		std::vector<std::string> careTypes;
		std::map<std::string, std::string>::const_iterator it = properties.find("supportedCareTypes");
		if (it == properties.end()) {
			return careTypes;
		}

		std::string token;
		std::istringstream stream(it->second);
		while (std::getline(stream, token, ',')) {
			if (!token.empty()) {
				careTypes.push_back(token);
			}
		}
		return careTypes;
	}

	int getStateDurationSeconds(const std::string &stateName, int defaultSeconds) const {
		return getNumericProperty("stateDuration." + toLowerKey(stateName), defaultSeconds);
	}

	void setStateDurationSeconds(const std::string &stateName, int seconds) {
		properties["stateDuration." + toLowerKey(stateName)] = std::to_string(seconds);
	}

	int getCareIntervalSeconds(const std::string &careType, int defaultSeconds) const {
		return getNumericProperty("careInterval." + toLowerKey(careType), defaultSeconds);
	}

	void setCareIntervalSeconds(const std::string &careType, int seconds) {
		properties["careInterval." + toLowerKey(careType)] = std::to_string(seconds);
	}

	static std::string sanitizeNumericString(const std::string &value, const std::string &fallback) {
		std::string digits;
		for (std::string::const_iterator it = value.begin(); it != value.end(); ++it) {
			if (std::isdigit(static_cast<unsigned char>(*it))) {
				digits.push_back(*it);
			}
		}
		if (digits.empty()) {
			return fallback;
		}
		return digits;
	}

protected:
	int getNumericProperty(const std::string &key, int defaultValue) const {
		std::map<std::string, std::string>::const_iterator it = properties.find(key);
		if (it == properties.end() || it->second.empty()) {
			return defaultValue;
		}

		const std::string &value = it->second;
		std::string digits;
		for (std::string::const_iterator vit = value.begin(); vit != value.end(); ++vit) {
			if (std::isdigit(static_cast<unsigned char>(*vit))) {
				digits.push_back(*vit);
			}
		}

		if (digits.empty()) {
			return defaultValue;
		}

		try {
			return std::stoi(digits);
		} catch (...) {
			return defaultValue;
		}
	}

	static std::string toLowerKey(const std::string &text) {
		std::string lowered = text;
		std::transform(lowered.begin(), lowered.end(), lowered.begin(), [](unsigned char c) {
			return static_cast<char>(std::tolower(c));
		});
		return lowered;
	}
};

#endif // PLANT_SPECIES_PROFILE_H
