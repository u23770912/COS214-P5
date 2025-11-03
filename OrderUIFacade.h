#ifndef ORDERUIFACADE_H
#define ORDERUIFACADE_H

#include <string>
#include <vector>

class Customer; // Forward declaration
class Order; // Forward declaration
class PlantProduct; // Forward declaration
class ConcreteOrderBuilder; // Forward declaration
class Pot; // Forward declaration

/**
 * @brief Facade class for Customer order UI operations
 * 
 * Handles all terminal UI operations and user interactions for the order system.
 * Separates UI concerns from Customer business logic following the Facade pattern.
 * Includes automatic discount calculation based on order size.
 * 
 * Extended to support pot ordering functionality with full customization options.
 */
class OrderUIFacade {
private:
    Customer* customer;
    
    // Pot ordering helper structure
    struct PotCustomization {
        std::string potType;      // "Clay", "Plastic", "Metal", "Glass", "Wooden"
        std::string size;         // "Small", "Medium", "Large"
        std::string shape;        // "Round", "Square", "Oval"
        bool hasDrainage;         // true/false
        std::string color;        // Color name (optional)
        std::string finish;       // Finish type (optional)
        std::string pattern;      // Pattern type (optional)
        double totalPrice;        // Final calculated price
        
        PotCustomization() : potType(""), size(""), shape(""), hasDrainage(true), 
                            color(""), finish(""), pattern(""), totalPrice(10.0) {}
    };
    
    // Private helper methods for pot ordering
    PotCustomization interactivePotConfiguration();
    Pot* createCustomizedPot(const PotCustomization& config);
    void displayPotCatalog();
    void displayPotCustomizationOptions();
    void displayPotConfigSummary(const PotCustomization& config);
    std::string getPotDescription(const PotCustomization& config);
    bool addColorToPot(Pot*& pot, PotCustomization& config);
    bool addFinishToPot(Pot*& pot, PotCustomization& config);
    bool addPatternToPot(Pot*& pot, PotCustomization& config);

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
    
    // Pot ordering methods - unified through facade
    bool addCustomizedPotToOrder(int quantity);
    bool addPlantWithCustomizedPot(int plantIndex, int quantity);
    
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
