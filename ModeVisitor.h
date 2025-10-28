#ifndef MODEVISTOR_H
#define MODEVISTOR_H

#include <string>

class StaffManager; // Forward declaration to avoid circular dependency
class PlantProduct; // Forward declaration

/**
 * @class ModeVisitor
 * @brief The Visitor interface for handling different StaffManager modes.
 */
class ModeVisitor {
public:
    virtual ~ModeVisitor() = default;
    virtual void processUpdate(StaffManager* manager, PlantProduct* plant, const std::string& commandType) = 0;
};

#endif // MODEVISTOR_H
