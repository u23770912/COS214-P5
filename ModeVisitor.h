#ifndef MODEVISITOR_H
#define MODEVISITOR_H

#include <string>

class StaffManager;
class PlantProduct;

/**
 * @class ModeVisitor
 * @brief The Visitor interface for handling different StaffManager modes.
 */
class ModeVisitor {
public:
    virtual ~ModeVisitor() = default;
    virtual void processUpdate(StaffManager* manager, PlantProduct* plant, const std::string& commandType) = 0;
};

#endif // MODEVISITOR_H