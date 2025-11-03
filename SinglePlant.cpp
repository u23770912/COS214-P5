#include "SinglePlant.h"
#include "Pot.h"
#include <sstream>
#include <iomanip>

SinglePlant::SinglePlant(const std::string& plantType, double price, int quantity, const std::string& size)
    : OrderItem(plantType, price, quantity), plantType(plantType), size(size), pot(nullptr) {}

SinglePlant::~SinglePlant() {
    // Clean up pot if it exists (we own it)
    delete pot;
}

double SinglePlant::getPrice() const {
    double totalPrice = price * quantity;
    
    // Add pot price if pot exists (using Decorator pattern pricing)
    if (pot) {
        totalPrice += pot->getPrice();
    }
    
    return totalPrice;
}

std::string SinglePlant::getDescription() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << quantity << "x " << plantType << " (" << size << ")";
    
    if (pot) {
        oss << " with ";
        // Get pot description by capturing print output
        std::ostringstream potDesc;
        std::streambuf* oldCoutBuf = std::cout.rdbuf(potDesc.rdbuf());
        pot->print();
        std::cout.rdbuf(oldCoutBuf);
        oss << potDesc.str();
    }
    
    return oss.str();
}

void SinglePlant::setPot(Pot* newPot) {
    // Delete old pot if it exists
    delete pot;
    
    // Set new pot (take ownership)
    pot = newPot;
}

void SinglePlant::removePot() {
    delete pot;
    pot = nullptr;
}

bool SinglePlant::hasPlantPot() const {
    return pot != nullptr;
}

Pot* SinglePlant::getPot() const {
    return pot;
}

std::string SinglePlant::getPlantType() const {
    return plantType;
}

std::string SinglePlant::getSize() const {
    return size;
}

// Deprecated methods - kept for backward compatibility
void SinglePlant::addPot(const std::string& potType, double potPrice) {
    // Legacy method - just adjust price
    // In new system, use setPot() with actual Pot object
    this->price += potPrice;
}

std::string SinglePlant::getPotType() const {
    if (pot) {
        return pot->getPotType();
    }
    return "";
}
