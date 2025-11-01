#include "SinglePlant.h"
#include <sstream>
#include <iomanip>

SinglePlant::SinglePlant(const std::string& plantType, double price, int quantity, const std::string& size)
    : OrderItem(plantType, price, quantity), plantType(plantType), size(size), hasPot(false), potType("") {}

SinglePlant::~SinglePlant() {}

double SinglePlant::getPrice() const {
    return price * quantity;
}

std::string SinglePlant::getDescription() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << quantity << "x " << plantType << " (" << size << ")";
    if (hasPot) {
        oss << " + " << potType << " pot";
    }
    return oss.str();
}

void SinglePlant::addPot(const std::string& potType, double potPrice) {
    this->potType = potType;
    this->price += potPrice;
    this->hasPot = true;
}

void SinglePlant::removePot() {
    if (hasPot) {
        // Note: We don't decrease price here as we don't know the pot price
        // In a real system, we'd track the pot price separately
        this->potType = "";
        this->hasPot = false;
    }
}

bool SinglePlant::hasPlantPot() const {
    return hasPot;
}

std::string SinglePlant::getPlantType() const {
    return plantType;
}

std::string SinglePlant::getSize() const {
    return size;
}

std::string SinglePlant::getPotType() const {
    return potType;
}
