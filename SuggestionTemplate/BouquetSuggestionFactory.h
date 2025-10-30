#ifndef BOUQUET_SUGGESTION_FACTORY_H
#define BOUQUET_SUGGESTION_FACTORY_H

#include "BouquetSuggestionTemplate.h"
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

/**
 * @brief Factory + Singleton: Creates and manages bouquet templates
 */
class BouquetSuggestionFactory {
private:
    std::map<std::string, BouquetSuggestionTemplate*> templates;
    
    BouquetSuggestionFactory();
    ~BouquetSuggestionFactory();
    BouquetSuggestionFactory(const BouquetSuggestionFactory&) = delete;
    BouquetSuggestionFactory& operator=(const BouquetSuggestionFactory&) = delete;
    
public:
    static BouquetSuggestionFactory& getInstance();
    
    BouquetSuggestionTemplate* getTemplate(const std::string& eventType);
    std::vector<std::string> getAvailableEvents() const;
    void registerTemplate(const std::string& eventType, BouquetSuggestionTemplate* tmpl);
};

#endif // BOUQUET_SUGGESTION_FACTORY_H