#ifndef PLANTINSTANCE_H
#define PLANTINSTANCE_H

#include "GreenhouseComponent.h"
#include <string>

// Forward declaration for CareCommand if it's in its own file
class CareCommand; 

class PlantInstance : public GreenhouseComponent {
private:
    CareCommand* cmd;
    std::string id;
    std::string info;

public:
    PlantInstance(std::string id, std::string description);
    ~PlantInstance();
    std::string getId();
    std::string getInfo();
    CareCommand* getCmd();
    void setCmd(CareCommand* cmd);
    void add(GreenhouseComponent* item) override;
    void print() override;
};

#endif
