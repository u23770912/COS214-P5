#ifndef PLANTDATA_H
#define PLANTDATA_H

#include <iostream>

class PlantData {
    private: 
        std::string id;
        std::string info;
        std::string type;
        std::string category;
        std::string species;
        double price;

    public:
        PlantData();
        PlantData(std::string id, std::string info, std::string type, std::string category , double price);
        void setId(std::string id);
        void setInfo(std::string info);
        void setCategory(std::string category);
        void setType(std::string type);
        void setSpecies(std::string species);
        std::string getId();
        std::string getInfo();
        std::string getType();
        std::string getSpecies();
        std::string getCategory();
        double getPrice();
};


#endif