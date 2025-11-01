#ifndef EFTADAPTEE_H
#define EFTADAPTEE_H

#include <string>

class EFTAdaptee {
public:
    EFTAdaptee();
    ~EFTAdaptee();
    bool processEFTTransaction(const std::string& bankAccount, double amount, std::string& outRef);
};

#endif // EFTADAPTEE_H