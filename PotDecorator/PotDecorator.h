#ifndef POTDECORATOR_H
#define POTDECORATOR_H

#include "Pot.h"
#include <string>

/**
 * @class PotDecorator
 * @brief Abstract decorator for pot customization (Decorator Pattern)
 * 
 * @details Enables dynamic addition of features to pots without modifying
 *          existing pot classes. InventoryManager stores Pot* which can be
 *          either base pots or decorated pots.
 */
class PotDecorator : public Pot {
protected:
    Pot* wrappedPot;
    double decorationPrice;

public:
    PotDecorator(Pot* pot, double price = 0.0);
    virtual ~PotDecorator();
    
    virtual double getPrice() const;
    virtual void print() override = 0;
};

#endif // POTDECORATOR_H