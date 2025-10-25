#ifndef COMMAND_H
#define COMMAND_H

#include "../PlantProduct.h"

#include <string>
#include <map>
#include <iostream>

/**
 * @brief Abstract Command class (Command pattern)
 * Defines the interface for executing and undoing commands
 */

class Command {

    private:
        //static map of command prototypes, keyed by a string type
        static std::map<std::string, Command*> prototypes;

    protected:
        PlantProduct* receiver; // The plant that will receive the command

    public:
        virtual ~Command() = default;

        // Command interface
        virtual void execute() = 0;
        virtual std::string getType() const = 0;
        virtual std::string getRequiredRole() const = 0;
        virtual Command* clone() const = 0;
        static void registerCommand(const std::string& type, Command* prototype);
        static Command* createCommand(const std::string& type);
        void setReceiver(PlantProduct* plant);
        virtual void undo();
};

#endif