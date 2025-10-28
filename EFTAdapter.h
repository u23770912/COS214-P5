#ifndef EFTADAPTER_H
#define EFTADAPTER_H

#include "PaymentProcessor.h"
#include <string>
#include <iostream>

class EFTAdaptee;

class EFTAdapter : public PaymentProcessor {
private:
    EFTAdaptee* adaptee;

public:
    EFTAdapter(EFTAdaptee* adaptee);
    virtual ~EFTAdapter();

    virtual bool processPayment(double amount, const std::string& customerId, const std::string payload) override;
};

#endif // EFTADAPTER_H
