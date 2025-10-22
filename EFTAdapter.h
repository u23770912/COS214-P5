#ifndef EFTAdapter_H
#define EFTAdapter_H

#include "PaymentProcessor.h"

#include <string>
#include <iostream>

using namespace std;
class EFTAdaptee;

class EFTAdapter : public PaymentProcessor {
private:
    EFTAdaptee* adaptee;

public:
    EFTAdapter(EFTAdaptee* adaptee);
    virtual ~EFTAdapter();
    virtual void processPayment(double amount, const string& customerId, const string payload) override;
};

#endif // EFTAdapter_H