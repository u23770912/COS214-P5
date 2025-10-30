// CreditCardAdapter.h
#ifndef CREDITCARDADAPTER_H
#define CREDITCARDADAPTER_H

#include "PaymentProcessor.h"
#include <string>

class CreditCardAdaptee;

class CreditCardAdapter : public PaymentProcessor {
private:
    CreditCardAdaptee* adaptee;
public:
    explicit CreditCardAdapter(CreditCardAdaptee* adaptee);
    ~CreditCardAdapter() override;
    bool processPayment(double amount, const std::string& customerId, const std::string& payload) override;
};

#endif // CREDITCARDADAPTER_H test_memento_adapter
