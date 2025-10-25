#ifndef CARE_STRATEGY_H
#define CARE_STRATEGY_H

#include <string>

// Generic Strategy Interface
class CareStrategy {
public:
    virtual ~CareStrategy() {}
    // The 'execute' method now takes a generic property string.
    virtual void execute(const std::string& property) = 0;
};

#endif // CARE_STRATEGY_H
