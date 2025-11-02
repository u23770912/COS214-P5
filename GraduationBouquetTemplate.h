#ifndef GRADUATION_BOUQUET_TEMPLATE_H
#define GRADUATION_BOUQUET_TEMPLATE_H

#include "BouquetSuggestionTemplate.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

class GraduationBouquetTemplate : public BouquetSuggestionTemplate {
public:
    GraduationBouquetTemplate() { eventType = "Graduation"; }
    
protected:
    std::string getEventEmoji() const override { return "🎓"; }
    
    std::string getEventMessage() const override { 
        return "Celebrate achievements with vibrant bouquets!"; 
    }
    
    BouquetSuggestion createStandardBouquet() const override {
        BouquetSuggestion bouquet;
        setBaseProperties(bouquet, "Success Celebration",
            {"Sunflower", "Rose", "Gerbera Daisy"},
            {5, 10, 8},
            "Yellow & Red",
            85.00,
            "Bright colors symbolizing achievement and new beginnings");
        return bouquet;
    }
    
    BouquetSuggestion createPremiumBouquet() const override {
        BouquetSuggestion bouquet;
        setBaseProperties(bouquet, "Scholar's Pride",
            {"Iris", "Tulip", "Rose", "Lily"},
            {12, 10, 8, 6},
            "Blue & Gold",
            120.00,
            "Represents wisdom, knowledge, and accomplishment");
        return bouquet;
    }
    
    BouquetSuggestion createBudgetBouquet() const override {
        BouquetSuggestion bouquet;
        setBaseProperties(bouquet, "Bright Future",
            {"Gerbera Daisy", "Carnation", "Rose"},
            {8, 10, 6},
            "Mixed Bright Colors",
            65.00,
            "Cheerful blooms for celebrating success");
        return bouquet;
    }
    
    std::string getWrappingStyle(const std::string& tier) const override {
        if (tier == "premium") return "Academic Colors with Tassel & Diploma Ribbon";
        if (tier == "budget") return "Simple Wrap with Congratulations Tag";
        return "Bold Ribbon with Graduation Cap Charm";
    }
    
    void customizeForEvent(BouquetSuggestion& bouquet) const override {
        bouquet.occasion = "Graduation Ceremony";
    }
};

#endif // GRADUATION_BOUQUET_TEMPLATE_H