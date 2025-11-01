#include "GreenhouseSystem.h"

GreenhouseSystem::GreenhouseSystem(): root(NULL), currentCategory(NULL){

}

GreenhouseSystem::~GreenhouseSystem() {
    delete root;
}

void GreenhouseSystem::reset(){
    if (root){
        delete root;
        root = NULL;
    }

    
    root = new PlantGroup("Greenhouse");
    currentCategory = root;

}


GreenhouseBuilder* GreenhouseSystem::addCategory(std::string name){

    GreenhouseComponent* item = new PlantGroup(name);
    root->add(item);

    currentCategory = item;

    return this;
}

GreenhouseBuilder* GreenhouseSystem::addType(std::string name){

    GreenhouseComponent* plantType = new PlantGroup(name);
    
    currentCategory->add(plantType);
    currentCategory = plantType;

    return this;
}


GreenhouseBuilder* GreenhouseSystem::addSpecies(std::string name){
    
    GreenhouseComponent* species = new PlantGroup(name);

    currentCategory->add(species);
    // currentCategory = species;

    return this;
}

// void GreenhouseSystem::addPlant(std::string id, std::string info){

// }


GreenhouseComponent* GreenhouseSystem::getGreenhouse(){
    return this->root;
}










// #include "GreenhouseSystem.h"

// GreenhouseSystem::GreenhouseSystem(): root(NULL), currentCategory(NULL){

// }

// GreenhouseSystem::~GreenhouseSystem() {
//     delete root;
// }

// void GreenhouseSystem::reset(){
//     if (root){
//         delete root;
//         root = NULL;
//     }

    
//     root = new PlantGroup("root");
//     currentCategory = root;

// }


// GreenhouseBuilder* GreenhouseSystem::addCategory(std::string name){

//     if (!root) reset();

//     GreenhouseComponent* item = new PlantGroup(name);
//     root->add(item);

//     currentCategory = item;

//     return this;
// }

// GreenhouseBuilder* GreenhouseSystem::addType(std::string type){

//     // cant add type before category
//     if (!currentCategory) {
//         std::cerr << "Error: addType called before addCategory!" << std::endl;
//         return this;
//     }


//     GreenhouseComponent* plantType = new PlantGroup(type);
    
//     currentCategory->add(plantType);
//     currentCategory = plantType;

//     return this;
// }


// GreenhouseBuilder* GreenhouseSystem::addSpecies(std::string type){
    
//     if (!currentCategory) {
//         std::cerr << "Error: addSpecies called before addCategory or addType!" << std::endl;
//         return this;
//     }

//     GreenhouseComponent* species = new PlantGroup(type);

//     currentCategory->add(species);
//     // currentCategory = species;

//     return this;
// }

// // void GreenhouseSystem::addPlant(std::string id, std::string info){

// // }


// GreenhouseComponent* GreenhouseSystem::getGreenhouse(){
//     return this->root;
// }



