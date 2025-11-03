// CashAdaptee.cpp
#include "CashAdaptee.h"
#include <string>

CashAdaptee::CashAdaptee() {}
CashAdaptee::~CashAdaptee() {}

bool CashAdaptee::processCashTransaction(double amount, std::string& receiptId)
{
    static int counter = 0;
    receiptId = "CASH-" + std::to_string(++counter);
    // For demo: always succeed
    return true;
}
