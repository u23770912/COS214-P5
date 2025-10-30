#include "PlasticPot.h"

PlasticPot::PlasticPot(std::string size, std::string shape, bool drainage): Pot(size, shape, drainage){

}

void PlasticPot::print(){
        std::cout << "Plastic Pot: [ Size: " << this->getSize() << ", Shape: " << this->getShape() << ", Drainage: " << this->getDrainage() << " ]" << std::endl;

}