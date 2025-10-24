#ifndef GREENHOUSESTAFF_H
#define GREENHOUSESTAFF_H

#include "StaffChainHandler.h"

class GreenhouseStaff : public StaffChainHandler {
public:
    virtual ~GreenhouseStaff() {}
    // This class is abstract and does not implement handleCommand.
    // Concrete subclasses like Gardener will implement it.
};

#endif // GREENHOUSESTAFF_H
