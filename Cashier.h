#ifndef Cashier_H
#define Cashier_H

#include "SalesFloorStaff.h"

/**
 * @class Cashier
 * @brief A Concrete Handler for sales floor tasks.
 * 
 * This is an instantiable class. The chain of responsibility for the
 * sales team will be composed of instances of Cashier.
 */
class Cashier : public SalesFloorStaff {
public:
    void handleCommand(Command* command) override;
};

#endif // Cashier_H
