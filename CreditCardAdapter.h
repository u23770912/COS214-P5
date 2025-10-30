// CreditCardAdapter.h
#ifndef CREDITCARDADAPTER_H
#define CREDITCARDADAPTER_H

#include "PaymentProcessor.h"
#include <string>

class CreditCardAdaptee;

/**
 * @brief Adapter for credit/debit card payment integration
 *
 * Translates the generic PaymentProcessor interface into calls against
 * the legacy CreditCardAdaptee implementation.
 */
class CreditCardAdapter : public PaymentProcessor {
private:
    /**
     * @brief Underlying credit card processing provider
     */
    CreditCardAdaptee* adaptee;
public:
    /**
     * @brief Construct a CreditCardAdapter
     * @param adaptee Pointer to the concrete credit card provider
     */
    explicit CreditCardAdapter(CreditCardAdaptee* adaptee);

    /**
     * @brief Destructor
     */
    ~CreditCardAdapter() override;

    /**
     * @brief Process a credit card payment
     * @param amount Amount to charge
     * @param customerId Customer identifier
     * @param payload Card details formatted as "card;expiry;cvc"
     * @return true on success, false otherwise
     */
    bool processPayment(double amount, const std::string& customerId, const std::string& payload) override;
};

#endif // CREDITCARDADAPTER_H test_memento_adapter
