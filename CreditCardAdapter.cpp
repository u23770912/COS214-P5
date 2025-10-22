#include "CreditCardAdapter.h"

CreditCardAdapter::CreditCardAdapter(CreditCardAdaptee* adaptee) : adaptee(adaptee)
{}

CreditCardAdapter::~CreditCardAdapter()
{}

void CreditCardAdapter::processPayment(double amount, const string& customerId, const string payload)
{}
