// CreditCardAdaptee.cpp
#include "CreditCardAdaptee.h"
#include <string>

CreditCardAdaptee::CreditCardAdaptee() {}
CreditCardAdaptee::~CreditCardAdaptee() {}

bool CreditCardAdaptee::processCreditCardTransaction(const std::string& cardNumber,const std::string& expiry,const std::string& cvc,double amount,std::string& receiptId)
{
    static int counter = 0;
    receiptId = "CC-" + std::to_string(++counter);
    // For demo: always succeed
    return true;
}
