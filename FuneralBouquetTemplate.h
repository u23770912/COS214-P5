#ifndef FUNERAL_BOUQUET_TEMPLATE_H
#define FUNERAL_BOUQUET_TEMPLATE_H

#include "BouquetSuggestionTemplate.h"

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

class FuneralBouquetTemplate : public BouquetSuggestionTemplate {
public:
    FuneralBouquetTemplate() { eventType = "Funeral"; }
    
protected:
    std::string getEventEmoji() const override { return "🕊️"; }
    
    std::string getEventMessage() const override { 
        return "Express sympathy with respectful arrangements"; 
    }
    
    BouquetSuggestion createStandardBouquet() const override {
        BouquetSuggestion bouquet;
        setBaseProperties(bouquet, "Peaceful Remembrance",
            {"White Lily", "Chrysanthemum", "Rose"},
            {10, 15, 8},
            "Pure White",
            120.00,
            "Symbolizes peace, purity, and eternal rest");
        return bouquet;
    }
    
    BouquetSuggestion createPremiumBouquet() const override {
        BouquetSuggestion bouquet;
        setBaseProperties(bouquet, "Eternal Grace",
            {"White Orchid", "Lily", "Baby's Breath", "Gladiolus"},
            {12, 15, 25, 10},
            "White & Cream",
            160.00,
            "Honors life with dignity and grace");
        return bouquet;
    }
    
    BouquetSuggestion createBudgetBouquet() const override {
        BouquetSuggestion bouquet;
        setBaseProperties(bouquet, "Heartfelt Sympathy",
            {"Carnation", "Rose", "Baby's Breath"},
            {15, 8, 20},
            "White & Soft Pink",
            85.00,
            "Conveys deep condolences and remembrance");
        return bouquet;
    }
    
    std::string getWrappingStyle(const std::string& tier) const override {
        if (tier == "premium") return "Reverent White Silk with Sympathy Card";
        if (tier == "budget") return "Simple White Paper";
        return "White Satin Ribbon";
    }
    
    void customizeForEvent(BouquetSuggestion& bouquet) const override {
        bouquet.occasion = "Memorial Service";
    }
};

#endif // FUNERAL_BOUQUET_TEMPLATE_H
