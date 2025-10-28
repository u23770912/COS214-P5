#ifndef CUSTOMER_H
#define CUSTOMER_H
#include <string>
#include <vector>

class OrderBuilder; // Forward declaration
class Order; // Forward declaration
class PlaceOrderCommand; // Forward declaration
class PlantProduct; // Forward declaration
class StaffManager; // Forward declaration

class Customer {
    private:
        std::string name;
        std::string email;
        std::string cellPhone;

        OrderBuilder* orderBuilder;
        Order* orderProduct;
        PlaceOrderCommand* placeOrderCommand;
        
        // Observer pattern - Staff management
        StaffManager* staffObserver;
       
    public:
        Customer(const std::string& name, const std::string& email, const std::string& cellPhone = "");
        ~Customer();

        std::string getName() const;
        std::string getEmail() const;
        std::string getCellPhone() const;

        // Enhanced order creation methods
        Order* createOrder(); // Creates new order and returns pointer for building
        bool executeOrder(); // executes the PlaceOrderCommand and returns success status
        
        // Interactive order building methods
        void displayAvailableItems();
        bool addPlantToOrder(int plantIndex, int quantity = 1);
        bool addBundleToOrder(const std::string& bundleName, const std::vector<int>& plantIndices, double discount = 10.0);
        void viewCurrentOrder();
        bool finalizeOrder();
        
        // Helper method to access the order builder
        class ConcreteOrderBuilder* getOrderBuilder();
        
        // Staff interaction methods - Observer pattern
        void setStaffObserver(StaffManager* staff);
        void notifyStaffOfInteraction(const std::string& interactionType, const std::string& details = "");
        bool requestStaffValidation(Order* order);
        
    private:
        // Helper methods for inventory management
        std::vector<PlantProduct*> getAvailablePlantsFromInventory();
        void displayPlantDetails(const PlantProduct* plant, int index);
};



#endif