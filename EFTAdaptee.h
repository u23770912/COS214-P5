#ifndef EFTADAPTEE_H
#define EFTADAPTEE_H

#include <string>
#include "iostream"

using namespace std;

class EFTAdaptee {
public:
    EFTAdaptee();
    ~EFTAdaptee();
    bool processEFTTransaction(const std::string& bankAccount, double amount, std::string& outRef);
};

#endif // EFTADAPTEE_H