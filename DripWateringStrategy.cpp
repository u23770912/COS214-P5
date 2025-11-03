#include "DripWateringStrategy.h"

void DripWateringStrategy::applyCare(int amount, const std::string &careType){
    if (careType == "drip" || careType == "water" || careType == "watering")
    {
        std::cout << "Applying " << amount << " ml of water via Drip System." << std::endl;
    }
    else
    {
        std::cout << "DripWateringStrategy does not handle care type: " << careType << std::endl;
    }
}

std::string DripWateringStrategy::getName() const{
    return "Drip Watering";
}