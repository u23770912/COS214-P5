#ifndef CREDITCARDADAPTEE_H
#define CREDITCARDADAPTEE_H

#include <string>
#include "iostream"

using namespace std;

class CreditCardAdaptee {
public:
    CreditCardAdaptee();
    ~CreditCardAdaptee();
    bool processCreditCardTransaction(const string& cardNumber, const string& expiry, const string& cvc, double amount, string& ReceiptId);
};

#endif // CREDITCARDADAPTEE_H