// CashAdapter.h
#ifndef CASHADAPTER_H
#define CASHADAPTER_H

#include "PaymentProcessor.h"
#include <string>

class CashAdaptee;

/**
 * @brief Adapter that wraps a legacy cash payment system (CashAdaptee)
 *
 * Implements the PaymentProcessor interface so the rest of the application
 * can process cash payments through a uniform API.
 */
class CashAdapter : public PaymentProcessor {
private:
    /**
     * @brief Pointer to the legacy cash system implementation
     */
    CashAdaptee* adaptee;
public:
    /**
     * @brief Construct a CashAdapter that delegates to the given adaptee
     * @param adaptee Pointer to a CashAdaptee implementation (ownership retained by caller)
     */
    explicit CashAdapter(CashAdaptee* adaptee);

    /**
     * @brief Destructor
     */
    ~CashAdapter() override;

    /**
     * @brief Process a cash payment
     * @param amount Amount to charge
     * @param customerId An identifier for the customer (may be unused by cash)
     * @param payload Additional details (unused for cash)
     * @return true on successful processing, false otherwise
     */
    bool processPayment(double amount, const std::string& customerId, const std::string& payload) override;
};

#endif // CASHADAPTER_H
