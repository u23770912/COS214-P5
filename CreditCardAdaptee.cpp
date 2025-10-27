#include "CreditCardAdaptee.h"

CreditCardAdaptee::CreditCardAdaptee()
{}

CreditCardAdaptee::~CreditCardAdaptee()
{}

bool CreditCardAdaptee::processCreditCardTransaction(const string& cardNumber, const string& expiry, const string& cvc, double amount, string& ReceiptId)
{
    static int counter = 0;
    ReceiptId = "CC-" + to_string(++counter);
    return true;
}