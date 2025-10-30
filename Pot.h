#ifndef POT_H
#define POT_H

#include <iostream>
#include <string>

// enum size
// {
//     small,
//     medium,
//     large
// };

class Pot
{
private:
    std::string size;
    std::string shape;
    bool drainage;

public:
    Pot(std::string size, std::string shape, bool drainage);
    virtual ~Pot();
    virtual void print() = 0;
    std::string getSize();
    bool getDrainage();
    std::string getShape();
};

#endif