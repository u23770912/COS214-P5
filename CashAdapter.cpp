#include "CashAdapter.h"
#include "CashAdaptee.h"

CashAdapter::CashAdapter(CashAdaptee* adaptee)
{
    this->adaptee = adaptee;
}

CashAdapter::~CashAdapter()
{}

void CashAdapter::processPayment(double amount, const string& customerId, const string payload)
{
    if (payload == "CASH" || payload.empty())
    {
        string receipt;
        bool success = adaptee->processCashTransaction(amount, receipt);
        if (success)
        {
            cout << "[CashAdapter] Cash payment processed for " << customerId
                << ", amount: R" << amount << ", receipt: " << receipt << endl;
        }
    }
    cout << "[CashAdapter] Unsupported payment payload for cash payment." << endl;
}
