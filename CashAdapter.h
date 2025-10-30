// CashAdapter.h
#ifndef CASHADAPTER_H
#define CASHADAPTER_H

#include "PaymentProcessor.h"
#include <string>

class CashAdaptee;

class CashAdapter : public PaymentProcessor {
private:
    CashAdaptee* adaptee;
public:
    explicit CashAdapter(CashAdaptee* adaptee);
    ~CashAdapter() override;
    bool processPayment(double amount, const std::string& customerId, const std::string& payload) override;
};

#endif // CASHADAPTER_H
