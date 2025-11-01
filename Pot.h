#ifndef POT_H
#define POT_H

#include <iostream>
#include <string>

/**
 * @class Pot
 * @brief Abstract base class representing a plant pot with various properties
 *
 * This class serves as the base for all types of pots (clay, glass, metal, etc.).
 * It stores common properties like size, shape, and drainage capability.
 */

class Pot
{
private:
    std::string size;
    std::string shape;
    bool drainage;

public:
    /**
     * @brief Constructs a Pot object
     * @param size The size of the pot
     * @param shape The shape of the pot
     * @param drainage Whether the pot has drainage holes
     */

    Pot(std::string size, std::string shape, bool drainage);
    virtual ~Pot();
    virtual void print() = 0;
    std::string getSize();
    std::string getDrainage();
    std::string getShape();
    virtual std::string getPotType() const = 0;
};

#endif