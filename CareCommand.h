#ifndef CARE_COMMAND_H
#define CARE_COMMAND_H

class PlantProduct;

class CareCommand {
protected:
    PlantProduct* plantReceiver;

public:
    CareCommand(PlantProduct* receiver);

    virtual ~CareCommand() = default;
    
    virtual void execute() = 0;
    
    PlantProduct* getPlantReceiver() const;
};

#endif