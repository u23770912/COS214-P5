#ifndef BOUQUET_SUGGESTION_H
#define BOUQUET_SUGGESTION_H

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>

using namespace std;

/**
 * @brief Represents a suggested bouquet configuration
 */
struct BouquetSuggestion {
    std::string eventType;
    std::string bouquetName;
    std::vector<std::string> flowerTypes;
    std::vector<int> quantities;
    std::string colorScheme;
    std::string wrappingStyle;
    double estimatedPrice;
    std::string significance;
    std::string occasion;
    
    std::string getDescription() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        
        oss << "🌸 " << bouquetName << "\n";
        oss << "   Event: " << eventType << "\n";
        oss << "   Colors: " << colorScheme << "\n";
        oss << "   Flowers: ";
        
        for (int i = 0; i < flowerTypes.size(); i++) {
            oss << quantities[i] << "x " << flowerTypes[i];
            if (i < flowerTypes.size() - 1) oss << ", ";
        }
        
        oss << "\n   Wrapping: " << wrappingStyle << "\n";
        oss << "   Significance: " << significance << "\n";
        oss << "   Price: $" << estimatedPrice;
        
        return oss.str();
    }
};

#endif // BOUQUET_SUGGESTION_H