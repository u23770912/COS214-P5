/**
 * @file GreenhouseDirector.h
 * @brief the file is part of the Builder pattern , so that the pattern can be completed and pass all the ecnapulate data onto the next ,without, effecting other people's code.
 */




#ifndef GREENHOUSEDIRECTOR_H
#define GREENHOUSEDIRECTOR_H

#include <string>
#include <vector>

#include "GreenhouseBuilder.h"
#include "PlantData.h"
#include "GreenhouseComponent.h"
#include "GreenhouseSystem.h"

class GreenhouseDirector
{
    private: 
        GreenhouseBuilder* builder;
        std::vector<PlantData*> plants;

    public:
         /**
     * @brief the  file  has tow main functions, the construct to  implement  the builder pattern , as well add a get method to get the plants and access the plants
     * 
     */
        GreenhouseDirector(GreenhouseBuilder* builder);
        // ~GreenhouseDirector();
         /**
     * @brief Construct for the builder's part.
     * @param builder take in a builder and pass the values  to constrcut the builfer.
     */
        GreenhouseComponent* construct();
        std::vector<PlantData*> getPlants();
        
        void setPlants(std::vector<PlantData*> plants);
         /**
        * @brief setPlants is that the primitive operation to construct the plants
        */
};


#endif