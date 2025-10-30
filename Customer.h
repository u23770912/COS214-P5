#ifndef CUSTOMER_H
#define CUSTOMER_H
#include <string>
#include <vector>
#include "CustomerSubject.h"

class OrderBuilder; // Forward declaration
class OrderDirector; // Forward declaration
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
        Order* orderProduct;
        PlaceOrderCommand* placeOrderCommand;
       
    public:
        Customer(const std::string& name, const std::string& email, const std::string& cellPhone = "");
        ~Customer();

        std::string getName() const;
        std::string getEmail() const;
        std::string getCellPhone() const;

        // Enhanced order creation methods
        Order* createOrder(); // Creates new order and returns pointer for building
        bool executeOrder(); // executes the PlaceOrderCommand and returns success status
        
        // Director-based construction methods
        Order* construct(); // Main construction method using Director
        Order* constructSimplePlantOrder(const std::string& plantType, int quantity);
        Order* constructPlantWithPotOrder(const std::string& plantType, const std::string& potType, int quantity);
        Order* constructBundleOrder(const std::string& bundleName, 
                                   const std::vector<std::string>& plantTypes,
                                   const std::vector<int>& quantities, 
                                   double discount);
        
        // Interactive order building methods
        void displayAvailableItems();
        bool addPlantToOrder(int plantIndex, int quantity = 1);
        bool addBundleToOrder(const std::string& bundleName, const std::vector<int>& plantIndices, double discount = 10.0);
        void viewCurrentOrder();
        bool finalizeOrder();
        
        // Helper method to access the order builder
        class ConcreteOrderBuilder* getOrderBuilder();
        
        // Observer pattern methods - Override from CustomerSubject
        void notifyInteraction(const std::string& interactionType, 
                             const std::string& details = "") override;
        bool requestValidation(Order* order) override;
        
        // Convenience methods (delegates to base class)
        void attachObserver(CustomerObserver* observer);
        void detachObserver(CustomerObserver* observer);
        
    private:
        // Helper methods for inventory management
        std::vector<PlantProduct*> getAvailablePlantsFromInventory();
        void displayPlantDetails(const PlantProduct* plant, int index);
};



#endif