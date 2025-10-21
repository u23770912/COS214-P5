#ifndef SALESFLOORSTAFF_H
#define SALESFLOORSTAFF_H

#include "StaffChainHandler.h"

/**
 * @class SalesFloorStaff
 * @brief An abstract handler representing the sales floor department/role.
 * 
 * This class serves as the base for all concrete workers on the sales floor,
 * such as SalesAssociates.
 */
class SalesFloorStaff : public StaffChainHandler {
public:
    virtual ~SalesFloorStaff() {}
    // This class is abstract and does not implement handleCommand.
    // Concrete subclasses like SalesAssociate will implement it.
};

#endif // SALESFLOORSTAFF_H
