#include "EFTAdapter.h"
#include "EFTAdaptee.h" 
#include <iostream>

EFTAdapter::EFTAdapter(EFTAdaptee* adaptee) : adaptee(adaptee) {}

EFTAdapter::~EFTAdapter() {}

bool EFTAdapter::processPayment(double amount, const std::string& customerId, const std::string& payload)
{
    if (payload == "EFT")
    {
        std::string ref;
        bool success = adaptee->processEFTTransaction("ZA123456789", amount, ref);
        if (success)
        {
            std::cout << "[EFTAdapter] EFT payment processed for " << customerId
                 << ", amount: R" << amount << ", reference: " << ref << std::endl;
            return true;
        }
    }
    return false;
}