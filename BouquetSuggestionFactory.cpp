#include "BouquetSuggestionFactory.h"
#include "WeddingBouquetTemplate.h"
#include "FuneralBouquetTemplate.h"
#include "GraduationBouquetTemplate.h"
#include "BirthdayBouquetTemplate.h"

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

BouquetSuggestionFactory::BouquetSuggestionFactory() {
    templates["Wedding"] = new WeddingBouquetTemplate();
    templates["Funeral"] = new FuneralBouquetTemplate();
    templates["Graduation"] = new GraduationBouquetTemplate();
    templates["Birthday"] = new BirthdayBouquetTemplate();
}

BouquetSuggestionFactory::~BouquetSuggestionFactory() {
    for (auto& pair : templates) {
        delete pair.second;
    }
}

BouquetSuggestionFactory& BouquetSuggestionFactory::getInstance() {
    static BouquetSuggestionFactory instance;
    return instance;
}

BouquetSuggestionTemplate* BouquetSuggestionFactory::getTemplate(const std::string& eventType) {
    auto it = templates.find(eventType);
    return (it != templates.end()) ? it->second : nullptr;
}

std::vector<std::string> BouquetSuggestionFactory::getAvailableEvents() const {
    std::vector<std::string> events;
    for (const auto& pair : templates) {
        events.push_back(pair.first);
    }
    return events;
}

void BouquetSuggestionFactory::registerTemplate(const std::string& eventType, 
                                                BouquetSuggestionTemplate* tmpl) {
    if (templates.find(eventType) != templates.end()) {
        delete templates[eventType];
    }
    templates[eventType] = tmpl;
}
