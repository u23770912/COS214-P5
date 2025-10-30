#ifndef WEDDING_BOUQUET_TEMPLATE_H
#define WEDDING_BOUQUET_TEMPLATE_H

#include "BouquetSuggestionTemplate.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

class WeddingBouquetTemplate : public BouquetSuggestionTemplate {
public:
    WeddingBouquetTemplate() { eventType = "Wedding"; }
    
protected:
    std::string getEventEmoji() const override { return "💍"; }
    
    std::string getEventMessage() const override { 
        return "Celebrate love with elegant wedding bouquets!"; 
    }
    
    BouquetSuggestion createStandardBouquet() const override {
        BouquetSuggestion bouquet;
        setBaseProperties(bouquet, "Classic Romance",
            {"Rose", "Baby's Breath", "Lily"},
            {12, 20, 6},
            "White & Ivory",
            150.00,
            "Timeless elegance symbolizing pure love");
        return bouquet;
    }
    
    BouquetSuggestion createPremiumBouquet() const override {
        BouquetSuggestion bouquet;
        setBaseProperties(bouquet, "Royal Elegance",
            {"Orchid", "Calla Lily", "Rose", "Peony"},
            {10, 8, 15, 12},
            "Deep Purple & White with Gold Accents",
            250.00,
            "Luxurious sophistication for grand celebrations");
        return bouquet;
    }
    
    BouquetSuggestion createBudgetBouquet() const override {
        BouquetSuggestion bouquet;
        setBaseProperties(bouquet, "Sweet Beginning",
            {"Rose", "Carnation", "Baby's Breath"},
            {8, 10, 15},
            "Soft Pink & White",
            95.00,
            "Beautiful simplicity for intimate ceremonies");
        return bouquet;
    }
    
    std::string getWrappingStyle(const std::string& tier) const override {
        if (tier == "premium") return "Velvet Ribbon with Crystal Brooch & Pearls";
        if (tier == "budget") return "Satin Ribbon";
        return "Elegant Lace with Ribbon";
    }
    
    void customizeForEvent(BouquetSuggestion& bouquet) const override {
        bouquet.occasion = "Bridal Ceremony";
    }
};

#endif // WEDDING_BOUQUET_TEMPLATE_H