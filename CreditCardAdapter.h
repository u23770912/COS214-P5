#ifndef CreditCardAdapter_H
#define CreditCardAdapter_H

#include "PaymentProcessor.h"

#include <string>
#include <iostream>

using namespace std;
class CreditCardAdaptee;
class CreditCardAdapter : public PaymentProcessor {
private:
    CreditCardAdaptee* adaptee;

public:
    CreditCardAdapter(CreditCardAdaptee* adaptee);
    virtual ~CreditCardAdapter();

    virtual bool processPayment(double amount, const string& customerId, const string payload) override;
};

#endif // CreditCardAdapter_H