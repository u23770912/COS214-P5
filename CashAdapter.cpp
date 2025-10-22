#include "CashAdapter.h"

CashAdapter::CashAdapter(CashAdaptee* adaptee) : adaptee(adaptee)
{}

CashAdapter::~CashAdapter()
{}

void CashAdapter::processPayment(double amount, const string& customerId, const string payload)
{}
