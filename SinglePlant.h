#ifndef SINGLEPLANT_H
#define SINGLEPLANT_H

#include "OrderItem.h"

// Forward declaration
class Pot;

/**
 * @brief Concrete leaf class representing a single plant (Composite pattern)
 * This is a Leaf in the Composite pattern
 */
class SinglePlant : public OrderItem {
private:
    std::string plantType;
    std::string size;
    Pot* pot;  // Using Pot* from Decorator pattern instead of string

public:
    SinglePlant(const std::string& plantType, double price, int quantity, 
                const std::string& size = "medium");
    ~SinglePlant();
    
    // Override OrderItem methods
    double getPrice() const override;
    std::string getDescription() const override;
    
    // SinglePlant specific methods
    void setPot(Pot* pot);  // Set pot (takes ownership)
    void removePot();
    bool hasPlantPot() const;
    Pot* getPot() const;
    std::string getPlantType() const;
    std::string getSize() const;
    
    // Deprecated - kept for backward compatibility
    void addPot(const std::string& potType, double potPrice);
    std::string getPotType() const;
};

#endif
