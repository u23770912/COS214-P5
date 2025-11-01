#ifndef COMMAND_H
#define COMMAND_H

#include "PlantProduct.h"
#include <iostream>
#include <string>
#include <map>


class PlantProduct;

class Command {
    private:
        // Static map to hold command prototypes, keyed by a string type.
        static std::map<std::string, Command*> prototypes;

    protected:
        PlantProduct* plantReceiver;

    public:
        Command();
       
        virtual ~Command();

        virtual void execute() = 0;
        
        virtual std::string getType() const = 0;
        
        virtual std::string getRequiredRole() const = 0;

        virtual Command* clone() const = 0;

        static void registerCommand(const std::string& type, Command* prototype);

        static Command* createCommand(const std::string& type);

        virtual void setReceiver(PlantProduct* plant);
        
        virtual PlantProduct* getReceiver() const;

        static void cleanupPrototypes();
};

#endif // COMMAND_H
