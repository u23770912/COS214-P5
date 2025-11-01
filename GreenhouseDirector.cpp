#include "GreenhouseDirector.h"

GreenhouseDirector::GreenhouseDirector(GreenhouseBuilder* builder): builder(builder){

}

std::vector<PlantData*> GreenhouseDirector::getPlants(){
    return this->plants;
}

void GreenhouseDirector::setPlants(std::vector<PlantData*> plants){
    this->plants = plants;
}

GreenhouseComponent* GreenhouseDirector::construct(){
   builder->reset();

    if (!plants.empty()) {
        std::string currentCategory = "";

        for (auto it = plants.begin(); it != plants.end(); ++it) {
            PlantData* plant = *it;

            // if new category, addCategory
            if (plant->getCategory() != currentCategory) {
                currentCategory = plant->getCategory();
                builder->addCategory(currentCategory);
            }

            // add species 
            builder->addSpecies(plant->getSpecies());
            // builder->addPlant(plant->getId(), plant->getInfo()); incase 
        }
    }

    return builder->getGreenhouse();

}