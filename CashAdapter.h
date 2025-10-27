#ifndef CASHADAPTER_H
#define CASHADAPTER_H

#include "PaymentProcessor.h"

#include <string>
#include <iostream>

using namespace std;
class CashAdaptee;

class CashAdapter : public PaymentProcessor {
private:
    CashAdaptee* adaptee;

public:
    CashAdapter(CashAdaptee* adaptee);
    virtual ~CashAdapter();

    virtual void processPayment(double amount, const string& customerId, const string payload) override;
};

#endif // CASHADAPTER_H