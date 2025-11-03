// CreditCardAdapter.cpp
#include "CreditCardAdapter.h"
#include "CreditCardAdaptee.h"
#include <iostream>

using namespace std;
#include <string>

CreditCardAdapter::CreditCardAdapter(CreditCardAdaptee* adaptee) : adaptee(adaptee) {}

CreditCardAdapter::~CreditCardAdapter() {}

bool CreditCardAdapter::processPayment(double amount, const std::string& customerId, const std::string& payload) {
    // expect payload format "cardNumber;expiry;cvc"
    size_t pos1 = payload.find(';');
    size_t pos2 = payload.rfind(';');
    if (pos1 == std::string::npos || pos2 == std::string::npos || pos1 == pos2) {
        cout << "[CreditCardAdapter] Invalid payload format for credit card." << endl;
        return false;
    }

    std::string card = payload.substr(0, pos1);
    std::string expiry = payload.substr(pos1 + 1, pos2 - pos1 - 1);
    std::string cvc = payload.substr(pos2 + 1);

    std::string receipt;
    bool ok = adaptee->processCreditCardTransaction(card, expiry, cvc, amount, receipt);
    if (ok) {
        cout << "[CreditCardAdapter] Credit card payment processed for " << customerId
             << ", amount: R" << amount << ", receipt: " << receipt << endl;
        return true;
    }

    cout << "[CreditCardAdapter] Credit card payment failed for " << customerId << endl;
    return false;
}