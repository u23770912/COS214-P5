#include "EFTAdaptee.h"

EFTAdaptee::EFTAdaptee()
{}

EFTAdaptee::~EFTAdaptee()
{}

bool EFTAdaptee::processEFTTransaction(const std::string& bankAccount, double amount, std::string& outRef)
{
    static int counter = 0;
    outRef = "EFT-" + to_string(++counter);
    return true;
}