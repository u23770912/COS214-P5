#include "PlantData.h"

PlantData::PlantData() : id(""), info(""), type(""), category(""), price(0.0){

}

PlantData::PlantData(std::string id, std::string info, std::string type, std::string category, double price): id(id), info(info), type(type), category(category), price(price){

}

void PlantData::setId(std::string id){
    this->id = id;
}

void PlantData::setInfo(std::string info){
    this->info = info;
}

void PlantData::setCategory(std::string category){
    this->category = category;
}

void PlantData::setType(std::string type){
    this->type = type;
}

void PlantData::setSpecies(std::string species){
    this->species = species;
}

std::string PlantData::getId(){
    return this->id;
}

std::string PlantData::getInfo(){
    return this->info;
}

std::string PlantData::getType(){
    return this->type;
}

std::string PlantData::getSpecies(){
    return this->species;
}

std::string PlantData::getCategory(){
    return this->category;
}

double PlantData::getPrice(){
    return this->price;
}