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
         /**
     * @brief constrcutor to make the plants wit the right plant data with the commands and request
     * @param id a tag to speret eh plants form other
     * @param info stares the infomation abou the plants
     * @param type what session or why type of plant to indicate ot what type of command to handle tha plants
     * @param category  the session  plants
     * @param price  the price if customer wants to but the plants
     */
        PlantData(std::string id, std::string info, std::string type, std::string category , double price);
        
    /**
     * @brief  a  constrctor that takes  complex objects
     * @param id [parameter description]
     */
        void setId(std::string id);
         /**
     * @brief a id to set with the plants
     * @param info [parameter description]
     */

      /** @brief Sets the plant ID */
        void setInfo(std::string info);
        
    /** @brief Sets the information field */
        /** @brief Sets the category */
        void setCategory(std::string category);
          /** @brief Sets the type */
        void setType(std::string type);
           /** @brief Sets the species */
        void setSpecies(std::string species);
        /** @return The plant ID */
        std::string getId();
        
    /** @return The plant information */
        std::string getInfo();
         /** @return The plant type */
        std::string getType();
        /** @return The plant species */
        std::string getSpecies();
        
    /** @return The plant category */

        std::string getCategory();
        /** @return The plant price */
        double getPrice();
};


#endif