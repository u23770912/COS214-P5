#ifndef BOUQUET_SUGGESTION_TEMPLATE_H
#define BOUQUET_SUGGESTION_TEMPLATE_H

#include "BouquetSuggestion.h"
#include <iostream>

#include <vector>
#include <string>
#include <sstream>

using namespace std;

/**
 * @brief Template Method Pattern: Abstract class defining the algorithm skeleton
 * 
 * The template method generateSuggestions() defines the structure:
 * 1. Display event message
 * 2. Create standard option
 * 3. Create premium option
 * 4. Create budget option
 * 5. Add event-specific decorations
 * 6. Return all suggestions
 */
class BouquetSuggestionTemplate {
protected:
    std::string eventType;
    
    // Primitive operations - must be implemented by subclasses
    virtual std::string getEventEmoji() const = 0;
    virtual std::string getEventMessage() const = 0;
    virtual BouquetSuggestion createStandardBouquet() const = 0;
    virtual BouquetSuggestion createPremiumBouquet() const = 0;
    virtual BouquetSuggestion createBudgetBouquet() const = 0;
    
    // Hook methods - can be overridden but have default implementations
    virtual void addSeasonalFlowers(BouquetSuggestion& bouquet) const {
        // Default: no seasonal additions
    }
    
    virtual std::string getWrappingStyle(const std::string& tier) const {
        if (tier == "premium") return "Luxury Gift Box with Ribbon";
        if (tier == "budget") return "Simple Paper Wrap";
        return "Elegant Ribbon Wrap";
    }
    
    virtual void customizeForEvent(BouquetSuggestion& bouquet) const {
        // Default: no special customization
    }
    
    // Helper method for setting common properties
    void setBaseProperties(BouquetSuggestion& bouquet, const std::string& name,
                          const std::vector<std::string>& flowers,
                          const std::vector<int>& quantities,
                          const std::string& colors,
                          double price,
                          const std::string& significance) const {
        bouquet.eventType = eventType;
        bouquet.bouquetName = name;
        bouquet.flowerTypes = flowers;
        bouquet.quantities = quantities;
        bouquet.colorScheme = colors;
        bouquet.estimatedPrice = price;
        bouquet.significance = significance;
    }
    
public:
    virtual ~BouquetSuggestionTemplate() {}
    
    /**
     * @brief TEMPLATE METHOD - Defines the algorithm skeleton
     * This is the core of the Template Method Pattern
     */
    std::vector<BouquetSuggestion> generateSuggestions() const {
        std::vector<BouquetSuggestion> suggestions;
        
        // Step 1: Display event information
        displayEventHeader();
        
        // Step 2: Create three tiers of bouquets (algorithm structure)
        BouquetSuggestion standard = createStandardBouquet();
        standard.wrappingStyle = getWrappingStyle("standard");
        customizeForEvent(standard);
        addSeasonalFlowers(standard);
        suggestions.push_back(standard);
        
        BouquetSuggestion premium = createPremiumBouquet();
        premium.wrappingStyle = getWrappingStyle("premium");
        customizeForEvent(premium);
        addSeasonalFlowers(premium);
        suggestions.push_back(premium);
        
        BouquetSuggestion budget = createBudgetBouquet();
        budget.wrappingStyle = getWrappingStyle("budget");
        customizeForEvent(budget);
        suggestions.push_back(budget);
        
        // Step 3: Display summary
        displaySummary(suggestions);
        
        return suggestions;
    }
    
    std::string getEventType() const { return eventType; }
    
protected:
    void displayEventHeader() const {
        std::cout << "\n" << getEventEmoji() << " " << getEventMessage() << "\n" << std::endl;
    }
    
    void displaySummary(const std::vector<BouquetSuggestion>& suggestions) const {
        std::cout << "Total options available: " << suggestions.size() << std::endl;
        std::cout << std::string(60, '=') << std::endl;
    }
};

#endif // BOUQUET_SUGGESTION_TEMPLATE_H