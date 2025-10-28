#ifndef CARE_COMMAND_H
#define CARE_COMMAND_H

#include "Command.h"
#include "PlantProduct.h"
#include <iostream>
#include <string>
#include <map>

// Forward declaration
class PlantProduct;

// Abstract Command (Command Pattern)
// Also acts as the Prototype factory.
class CareCommand : public Command
{
private:
    // Static map to hold command prototypes.
    static std::map<std::string, CareCommand *> prototypes;

protected:
    PlantProduct *plantReceiver;

public:
    // Constructor for prototypes (no receiver yet)
    CareCommand() : plantReceiver(nullptr) {}
    virtual ~CareCommand() {}

    virtual void execute() = 0;
    virtual std::string getType() const = 0;
    virtual std::string getRequiredRole() const = 0;

    // --- Prototype Pattern Methods ---
    virtual CareCommand *clone() const = 0;

    void setReceiver(PlantProduct *plant)
    {
        this->plantReceiver = plant;
    }

    PlantProduct *getReceiver() const
    {
        return plantReceiver;
    }

    // --- Static Factory Methods ---

    /**
     * @brief Registers a command prototype with the factory.
     * @param type The string identifier for the command.
     * @param prototype A pointer to a prototype instance of the command.
     */
    static void registerCommand(const std::string &type, CareCommand *prototype)
    {
        prototypes[type] = prototype;
    }

    /**
     * @brief Creates a new command by cloning a registered prototype.
     * @param type The string identifier for the command to create.
     * @return A new CareCommand instance, or nullptr if the type is not registered.
     */
    static CareCommand *createCommand(const std::string &type)
    {
        auto it = prototypes.find(type);
        if (it != prototypes.end())
        {
            return it->second->clone();
        }
        return nullptr;
    }
};

#endif // CARE_COMMAND_H