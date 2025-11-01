#include "PlantPot.h"

PlantPot::PlantPot(std::string id, std::string info) : id(id), info(info){

}


PlantPot::~PlantPot(){

}


std::string PlantPot::getId(){
    return this->id;
}

std::string PlantPot::getInfo(){
    return this->info;
}


void PlantPot::add(GreenhouseComponent* component){

}

void PlantPot::print(){
   std::cout << this->getId() << " - " << this->getInfo();
}