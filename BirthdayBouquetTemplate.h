#ifndef BIRTHDAY_BOUQUET_TEMPLATE_H
#define BIRTHDAY_BOUQUET_TEMPLATE_H

#include "BouquetSuggestionTemplate.h"

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

class BirthdayBouquetTemplate : public BouquetSuggestionTemplate {
public:
    BirthdayBouquetTemplate() { eventType = "Birthday"; }
    
protected:
    std::string getEventEmoji() const override { return "🎂"; }
    
    std::string getEventMessage() const override { 
        return "Make their day special with cheerful blooms!"; 
    }
    
    BouquetSuggestion createStandardBouquet() const override {
        BouquetSuggestion bouquet;
        setBaseProperties(bouquet, "Joy & Laughter",
            {"Gerbera Daisy", "Tulip", "Rose"},
            {10, 12, 8},
            "Rainbow Mix",
            75.00,
            "Bright and cheerful for happy celebrations");
        return bouquet;
    }
    
    BouquetSuggestion createPremiumBouquet() const override {
        BouquetSuggestion bouquet;
        setBaseProperties(bouquet, "Celebration Deluxe",
            {"Sunflower", "Lily", "Iris", "Rose"},
            {8, 10, 12, 10},
            "Bold Yellow & Orange with Purple",
            105.00,
            "Energetic and fun for milestone birthdays");
        return bouquet;
    }
    
    BouquetSuggestion createBudgetBouquet() const override {
        BouquetSuggestion bouquet;
        setBaseProperties(bouquet, "Sweet Surprise",
            {"Rose", "Carnation", "Baby's Breath"},
            {12, 10, 15},
            "Pink & White",
            55.00,
            "Gentle beauty perfect for any age");
        return bouquet;
    }
    
    std::string getWrappingStyle(const std::string& tier) const override {
        if (tier == "premium") return "Festive Box with Balloon & Birthday Banner";
        if (tier == "budget") return "Colorful Paper with Bow";
        return "Fun Patterned Paper with Birthday Tag";
    }
    
    void customizeForEvent(BouquetSuggestion& bouquet) const override {
        bouquet.occasion = "Birthday Party";
    }
    
    void addSeasonalFlowers(BouquetSuggestion& bouquet) const override {
        // Birthday bouquets can include seasonal surprises
        bouquet.significance += " (includes seasonal accents)";
    }
};

#endif // BIRTHDAY_BOUQUET_TEMPLATE_H