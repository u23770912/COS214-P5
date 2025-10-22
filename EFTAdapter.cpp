#include "EFTAdapter.h"

EFTAdapter::EFTAdapter(EFTAdaptee* adaptee) : adaptee(adaptee)
{}

EFTAdapter::~EFTAdapter()
{}

void EFTAdapter::processPayment(double amount, const string& customerId, const string payload)
{}
