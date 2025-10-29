#ifndef PAYMENT_PROCESSOR_H
#define PAYMENT_PROCESSOR_H

#include <string>

class PaymentProcessor {
public:
    virtual ~PaymentProcessor() = default;
    // payload is format-dependent: "CASH" or "cardNumber;expiry;cvc" or other
    virtual bool processPayment(double amount, const std::string& customerId, const std::string& payload) = 0;
};

#endif // PAYMENT_PROCESSOR_H
