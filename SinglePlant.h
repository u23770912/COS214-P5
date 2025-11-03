#ifndef SINGLEPLANT_H
#define SINGLEPLANT_H

#include "OrderItem.h"

/**
 * @brief Concrete leaf class representing a single plant (Composite pattern)
 * This is a Leaf in the Composite pattern
 */
class SinglePlant : public OrderItem {
private:
    std::string plantType;
    std::string size;
    bool hasPot;
    std::string potType;

public:
    SinglePlant(const std::string& plantType, double price, int quantity, 
                const std::string& size = "medium");
    ~SinglePlant();
    
    // Override OrderItem methods
    double getPrice() const override;
    std::string getDescription() const override;
    
    // SinglePlant specific methods
    void addPot(const std::string& potType, double potPrice);
    void removePot();
    bool hasPlantPot() const;
    std::string getPlantType() const;
    std::string getSize() const;
    std::string getPotType() const;
};

#endif
