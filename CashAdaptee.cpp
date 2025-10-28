#include "CashAdaptee.h"

CashAdaptee::CashAdaptee()
{}

CashAdaptee::~CashAdaptee()
{}

bool CashAdaptee::processCashTransaction(double ammount, string&  receiptId)
{
    static int counter = 0;
    receiptId = "CASH-" + to_string(++counter);
    return true; // always succeeds for demo purposes
}