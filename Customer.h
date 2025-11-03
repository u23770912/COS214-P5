#ifndef CUSTOMER_H
#define CUSTOMER_H
#include <string>
#include <vector>
#include <map>
#include "CustomerSubject.h"

class OrderHistory; // Forward declaration
class OrderBuilder; // Forward declaration
class OrderDirector; // Forward declaration
class OrderUIFacade; // Forward declaration
class Order; // Forward declaration
class PlaceOrderCommand; // Forward declaration
class PlantProduct; // Forward declaration
class CustomerObserver; // Forward declaration
class PaymentProcessor; // Forward declaration
class CashAdaptee; // Forward declaration
class CreditCardAdaptee; // Forward declaration
class EFTAdaptee; // Forward declaration
class StaffManager; // Forward declaration


/**
 * @brief ConcreteSubject in the Customer Observer pattern
 * Notifies observers (staff) about customer interactions and order events
 * Enhanced with Payment processing (Adapter pattern) and Order history (Memento pattern)
 */
class Customer : public CustomerSubject {
    private:
        std::string name;
        std::string email;
        std::string cellPhone;

        OrderBuilder* orderBuilder;
        OrderDirector* orderDirector;
        OrderUIFacade* uiFacade; // Facade for all UI operations
        Order* orderProduct;
        PlaceOrderCommand* placeOrderCommand;
        
        // Observer pattern - Staff management (Additional from salesfloor)
        StaffManager* staffObserver;

        // Memento pattern - order history
        OrderHistory* orderHistory;

        // Adapter pattern - payment processors
        CashAdaptee* cashSystem;
        CreditCardAdaptee* creditCardSystem;
        EFTAdaptee* eftSystem;
        std::map<std::string, PaymentProcessor*> paymentAdapters;

        void initializePaymentSystems();
        void cleanupPaymentSystems();
       
    public:
        Customer(const std::string& name, const std::string& email, const std::string& cellPhone = "");
        ~Customer();

        std::string getName() const;
        std::string getEmail() const;
        std::string getCellPhone() const;

        // Core order creation methods - Customer's primary responsibility
        Order* createOrder(); // Creates new order and returns pointer for building
        bool executeOrder(); // executes the PlaceOrderCommand and returns success status
        bool finalizeOrder(); // Finalizes the current order for execution
        
        // Director-based construction methods - Delegate complex construction
        Order* construct(); // Main construction method using Director
        Order* constructSimplePlantOrder(const std::string& plantType, int quantity);
        Order* constructPlantWithPotOrder(const std::string& plantType, const std::string& potType, int quantity);
        Order* constructBundleOrder(const std::string& bundleName, 
                                   const std::vector<std::string>& plantTypes,
                                   const std::vector<int>& quantities, 
                                   double discount);
        
        // Order building methods - Core business logic only (no UI)
        bool addPlantToOrder(int plantIndex, int quantity = 1);
        bool addBundleToOrder(const std::string& bundleName, const std::vector<int>& plantIndices);
        
        // UI methods - Delegated to facade (for compatibility with existing tests)
        void displayAvailableItems(); // Delegates to facade
        void viewCurrentOrder(); // Delegates to facade
        
        // Access methods for facade and components (Current implementation)
        OrderUIFacade* getUIFacade(); // Get UI facade for terminal operations
        class ConcreteOrderBuilder* getOrderBuilder(); // Access to order builder
        
        // Observer pattern methods - Override from CustomerSubject (Current implementation)
        void notifyInteraction(const std::string& interactionType, 
                             const std::string& details = "") override;
        bool requestValidation(Order* order) override;
        
        // Convenience methods (delegates to base class)
        void attachObserver(CustomerObserver* observer);
        void detachObserver(CustomerObserver* observer);
        
        // Staff interaction methods - Observer pattern (Convenience methods from salesfloor integration)
        void setStaffObserver(StaffManager* staff); // Convenience: sets staff and adds to observers
        void notifyStaffOfInteraction(const std::string& interactionType, const std::string& details = ""); // Delegates to notifyInteraction()
        bool requestStaffValidation(Order* order); // Convenience: delegates to requestValidation()

        // Memento pattern - order history methods
        void saveCurrentOrder();
        void restoreLastOrder();
        void viewOrderHistory();

    // Suggestion browsing (e.g., bouquet/event suggestions)
    void browseBouquetSuggestions(const std::string& eventType);

        // Adapter pattern - payment processing
        bool processPayment(const std::string& paymentType, double amount, const std::string& paymentDetails = "");
        void showPaymentOptions() const;
        bool isPaymentMethodSupported(const std::string& paymentType) const;
        bool executeOrderWithPayment(const std::string& paymentType, const std::string& paymentDetails = "");
    private:
        // Private helper methods for order management only
        void cleanupPreviousOrder(); // Helper to clean up previous orders
};

#endif // CUSTOMER_H