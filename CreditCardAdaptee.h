// CreditCardAdaptee.h
#ifndef CREDITCARDADAPTEE_H
#define CREDITCARDADAPTEE_H

#include <string>

class CreditCardAdaptee {
public:
    CreditCardAdaptee();
    ~CreditCardAdaptee();
    // returns true on success, writes receiptId
    bool processCreditCardTransaction(const std::string& cardNumber,const std::string& expiry,const std::string& cvc,double amount,std::string& receiptId);
};

#endif // CREDITCARDADAPTEE_H
