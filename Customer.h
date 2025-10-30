#ifndef CUSTOMER_H
#define CUSTOMER_H
#include <string>
#include <vector>
#include "CustomerSubject.h"

class OrderBuilder; // Forward declaration
class OrderDirector; // Forward declaration
class OrderUIFacade; // Forward declaration
class Order; // Forward declaration
class PlaceOrderCommand; // Forward declaration
class PlantProduct; // Forward declaration
class CustomerObserver; // Forward declaration

/**
 * @brief ConcreteSubject in the Customer Observer pattern
 * Notifies observers (staff) about customer interactions and order events
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
        
        // Access methods for facade and components
        OrderUIFacade* getUIFacade(); // Get UI facade for terminal operations
        class ConcreteOrderBuilder* getOrderBuilder(); // Access to order builder
        
        // Observer pattern methods - Override from CustomerSubject
        void notifyInteraction(const std::string& interactionType, 
                             const std::string& details = "") override;
        bool requestValidation(Order* order) override;
        
        // Convenience methods (delegates to base class)
        void attachObserver(CustomerObserver* observer);
        void detachObserver(CustomerObserver* observer);
        
    private:
        // Private helper methods for order management only
        void cleanupPreviousOrder(); // Helper to clean up previous orders
};



#endif