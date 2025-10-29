// CashAdaptee.h
#ifndef CASHADAPTEE_H
#define CASHADAPTEE_H

#include <string>

class CashAdaptee {
public:
    CashAdaptee();
    ~CashAdaptee();
    // returns true on success, writes receiptId
    bool processCashTransaction(double amount, std::string& receiptId);
};

#endif // CASHADAPTEE_H
