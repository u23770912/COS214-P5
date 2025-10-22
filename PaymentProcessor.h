#ifndef PAYMENT_PROCESSOR_H
#define PAYMENT_PROCESSOR_H

#include <iostream>
#include <string>

using namespace std;

class PaymentHandler;

class PaymentProcessor {
private:
    PaymentHandler* handler;
public:
    PaymentProcessor();
    virtual ~PaymentProcessor();
    void setHandler(PaymentHandler* handler);
    PaymentHandler* getHandler() const;
    virtual void processPayment(double amount, const string& customerId, const string payload) = 0;

};

#endif // PAYMENT_PROCESSOR_H