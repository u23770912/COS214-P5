#ifndef EFTADAPTER_H
#define EFTADAPTER_H

#include "PaymentProcessor.h"
#include <string>

class EFTAdaptee;  // Forward declaration

/**
 * @brief Adapter for Electronic Funds Transfer (EFT) payment systems
 *
 * Converts PaymentProcessor calls into the concrete EFTAdaptee API.
 */
class EFTAdapter : public PaymentProcessor {
private:
    /**
     * @brief Underlying EFT implementation
     */
    EFTAdaptee* adaptee;

public:
    /**
     * @brief Create an EFTAdapter that delegates to an EFTAdaptee
     * @param adaptee Pointer to the concrete EFT implementation
     */
    EFTAdapter(EFTAdaptee* adaptee);

    /**
     * @brief Destructor
     */
    virtual ~EFTAdapter();

    /**
     * @brief Process an EFT payment
     * @param amount Amount to transfer
     * @param customerId Identifier for the customer or account
     * @param payload Additional payload (e.g., account number)
     * @return true on success
     */
    virtual bool processPayment(double amount, const std::string& customerId, const std::string& payload) override;
};

#endif // EFTADAPTER_H