#ifndef CASHADAPTEE_H
#define CASHADAPTEE_H

#include <string>
#include "iostream"

using namespace std;

class CashAdaptee {
public:
    CashAdaptee();
    ~CashAdaptee();
    bool processCashTransaction(double ammount, string&  ReceiptId);
};

#endif // CASHADAPTEE_H