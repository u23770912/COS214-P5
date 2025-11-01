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

void PlantPot::print(const std::string& prefix, bool isLast){
   std::cout << prefix << (isLast ? "└── " : "├── ") 
              << "PlantPot ID: " << id << ", Info: " << info << std::endl;
}

void PlantPot::remove(GreenhouseComponent *)
{
    std::cerr << "Error: Cannot remove component from a PlantPot (leaf)." << std::endl;
}

void PlantPot::display()
{
   this->print("", true);
}

void PlantPot::getPlantCount()
{
    //
}
