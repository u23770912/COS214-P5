// CashAdapter.cpp
#include "CashAdapter.h"
#include "CashAdaptee.h"
#include <iostream>

using std::cout;
using std::endl;

CashAdapter::CashAdapter(CashAdaptee* adaptee) : adaptee(adaptee) {}
CashAdapter::~CashAdapter() = default;

bool CashAdapter::processPayment(double amount, const std::string& customerId, const std::string& payload)
{
    // treat "CASH" or empty as cash payment
    if (payload == "CASH" || payload.empty()) {
        std::string receipt;
        bool ok = adaptee->processCashTransaction(amount, receipt);
        if (ok) {
            cout << "[CashAdapter] Cash payment processed for " << customerId
                 << ", amount: R" << amount << ", receipt: " << receipt << endl;
            return true;
        }
        cout << "[CashAdapter] Cash payment failed for " << customerId << endl;
        return false;
    }

    // unsupported payload for cash adapter
    cout << "[CashAdapter] Unsupported payload for cash adapter: " << payload << endl;
    return false;
}
