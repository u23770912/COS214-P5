#ifndef CASHIER_H
#define CASHIER_H

#include "SalesFloorStaff.h"
#include "StaffMember.h"
#include "Order.h"

// Forward declarations
class Customer;

/**
 * @class Cashier
 * @brief Concrete Handler for sales floor tasks in the Chain of Responsibility pattern.
 * This class is instantiable and multiple Cashier instances form a chain
 * to handle concurrent sales floor operations.
 */
class Cashier : public SalesFloorStaff {
public:
    Cashier() : SalesFloorStaff() {}
    virtual ~Cashier() {}

    /**
     * @brief Handle a command in the Chain of Responsibility pattern
     * @param command The command to handle (takes ownership)
     */
    void handleCommand(Command* command) override;

    /**
     * @brief Validate a customer order before processing
     * @param order The order to validate
     * @param customer The customer placing the order
     * @return true if order is valid, false otherwise
     */
    bool validateOrder(Order* order, Customer* customer);

private:
    /**
     * @brief Process a MoveToSalesFloorCommand
     * Called by handleCommand when command type is MoveToSalesFloor
     */
    void processMoveToSalesFloor(Command* command);
};

#endif // CASHIER_H
