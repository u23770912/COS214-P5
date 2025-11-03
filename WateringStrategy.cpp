#include "WateringStrategy.h"
#include <iostream>

void WateringStrategy::applyCare(int amount, const std::string &careType)
{
    if (careType == "water" || careType == "watering")
    {
        std::cout << "Watering plant with " << amount << " ml of water." << std::endl;
    }
    else
    {
        std::cout << "WateringStrategy does not handle care type: " << careType << std::endl;
    }
}