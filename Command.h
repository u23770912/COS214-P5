#ifndef COMMAND_H
#define COMMAND_H

#include "PlantProduct.h"

#include <string>
#include <map>
#include <iostream>

class Command
{
private:
    // Static map to hold command prototypes, keyed by a string type.
    static std::map<std::string, Command *> prototypes;

public:
    virtual ~Command();

    virtual void execute() = 0;
    virtual std::string getType() const = 0;
    virtual std::string getRequiredRole() const = 0;

    virtual Command *clone() const = 0;

    static void registerCommand(const std::string &type, Command *prototype)
    {
        prototypes[type] = prototype;
    }

    static Command *createCommand(const std::string &type)
    {
        auto it = prototypes.find(type);
        if (it != prototypes.end())
        {
            return it->second->clone();
        }
        std::cout << "Error: Command type '" << type << "' not registered." << std::endl;
        return nullptr;
    }

    virtual void setReceiver(PlantProduct *plant);
    virtual PlantProduct *getReceiver() const;
};

#endif // COMMAND_H
