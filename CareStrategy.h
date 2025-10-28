#ifndef CARE_STRATEGY_H
#define CARE_STRATEGY_H

#include <string>

// Generic Strategy Interface
class CareStrategy {
public:
    virtual ~CareStrategy() {}
    //handle different care types with a numeric amount
   virtual void applyCare(int amount, const std::string& careType) = 0;
};

#endif // CARE_STRATEGY_H
