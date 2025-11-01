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
        std::string currentType = "";     // Added tracking for Type
        std::string currentSpecies = "";  // Added tracking for Species

        // NOTE: The PlantData must be sorted by Category, Type, and Species for this simple loop to work correctly.
        for (PlantData* plant : plants) {
            
            //Check/Add Category
            if (plant->getCategory() != currentCategory) {
                currentCategory = plant->getCategory();
                builder->addCategory(currentCategory);
                currentType = "";     // Reset lower levels
                currentSpecies = "";
            }

            //Check/Add Type
            if (plant->getType() != currentType) {
                currentType = plant->getType();
                builder->addType(currentType);
                currentSpecies = "";  // Reset lower level
            }

            // Check/Add Species
            if (plant->getSpecies() != currentSpecies) {
                currentSpecies = plant->getSpecies();
                builder->addSpecies(currentSpecies);
            }
            
            // Add the individual plant (the Leaf)
            builder->addPlant(plant); 
        }
    }

    return builder->getGreenhouse();
}