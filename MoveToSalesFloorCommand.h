#ifndef MOVE_TO_SALES_FLOOR_COMMAND_H
#define MOVE_TO_SALES_FLOOR_COMMAND_H

#include "Command.h"
#include <string>

class PlantProduct;

class MoveToSalesFloorCommand : public Command {
    private:
        PlantProduct* plantReceiver;
    public:
        MoveToSalesFloorCommand();
        ~MoveToSalesFloorCommand();
        
        void setReceiver(PlantProduct* plant) override;
        void execute() override;
        std::string getType() const override;
        std::string getRequiredRole() const override;
        Command* clone() const override;
};

#endif
