#ifndef ORDERUIFACADE_H
#define ORDERUIFACADE_H

#include <string>
#include <vector>

class Customer; // Forward declaration
class Order; // Forward declaration
class PlantProduct; // Forward declaration
class ConcreteOrderBuilder; // Forward declaration

/**
 * @brief Facade class for Customer order UI operations
 * 
 * Handles all terminal UI operations and user interactions for the order system.
 * Separates UI concerns from Customer business logic following the Facade pattern.
 * Includes automatic discount calculation based on order size.
 */
class OrderUIFacade {
private:
    Customer* customer;

public:
    // Public method for testing automatic discount logic
    double calculateAutomaticDiscount(int totalPlants) const;
    
    // Helper methods for UI display
    std::vector<PlantProduct*> getAvailablePlantsFromInventory();
    void displayPlantDetails(const PlantProduct* plant, int index);
    int countTotalPlantsInOrder(ConcreteOrderBuilder* builder) const;
    
public:
    OrderUIFacade(Customer* customer);
    ~OrderUIFacade();
    
    // Main UI methods - handle all customer-facing terminal operations
    void displayAvailableItems();
    void viewCurrentOrder();
    
    // Enhanced order building with automatic discount application
    bool addPlantToOrderWithAutoDiscount(int plantIndex, int quantity);
    bool addBundleToOrderWithAutoDiscount(const std::string& bundleName, 
                                         const std::vector<int>& plantIndices);
    
    // Display methods for order management
    void displayOrderCreationWelcome();
    void displayOrderExecutionSummary();
    void displayFinalOrderConfirmation();
    
    // Utility methods
    void displayDiscountInformation() const;
    void displayOrderInstructions() const;
    
    // Set customer reference (for flexibility)
    void setCustomer(Customer* customer);
    Customer* getCustomer() const;
};

#endif // ORDERUIFACADE_H
