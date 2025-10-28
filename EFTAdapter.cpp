#include "EFTAdapter.h"
#include "EFTAdaptee.h" 
#include <iostream>

using namespace std;

EFTAdapter::EFTAdapter(EFTAdaptee* adaptee) : adaptee(adaptee) {}
EFTAdapter::~EFTAdapter() {}

bool EFTAdapter::processPayment(double amount, const string& customerId, const string payload)
{
    if (payload == "EFT")
    {
        string ref;
        bool success = adaptee->processEFTTransaction("ZA123456789", amount, ref);
        if (success)
        {
            cout << "[EFTAdapter] EFT payment processed for " << customerId
                 << ", amount: R" << amount << ", reference: " << ref << endl;
            return true;
        }
    }
    return false;
}
