#include "CreditCardAdapter.h"
#include "CreditCardAdaptee.h"

CreditCardAdapter::CreditCardAdapter(CreditCardAdaptee* adaptee) : adaptee(adaptee)
{}

CreditCardAdapter::~CreditCardAdapter()
{}

bool CreditCardAdapter::processPayment(double amount, const string& customerId, const string payload)
{
    size_t pos1 = payload.find(';');
    size_t pos2 = payload.rfind(';');
    if (pos1 == string::npos || pos2 == string::npos || pos1 == pos2) 
    {
        return false;
    }

    string card = payload.substr(0, pos1);
    string expiry = payload.substr(pos1 + 1, pos2 - pos1 - 1);
    string cvc = payload.substr(pos2 + 1);

    string receipt;
    bool success = adaptee->processCreditCardTransaction(card, expiry, cvc, amount, receipt);
    if (success)
    {
        cout << "[CreditCardAdapter] Credit card payment processed for " << customerId
             << ", amount: R" << amount << ", receipt: " << receipt << endl;
        return true;
    }

    cout << "[CreditCardAdapter] Credit card payment failed for " << customerId << endl;
    return false;
}
