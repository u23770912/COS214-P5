# Builder-Director Pattern Implementation

## Overview

This document describes the implementation of the Builder-Director Pattern in the Customer Order System. The pattern separates the construction of complex orders from their representation, allowing the same construction process to create different types of orders while keeping the Customer code clean and simple.

## Pattern Components

### 1. Product (Order)
- **Role**: The complex object being built
- **Responsibility**: Represents the final order with items, customer information, and pricing
- **File**: `Order.h`, `Order.cpp`

### 2. Builder (OrderBuilder + ConcreteOrderBuilder)
- **Role**: Abstract interface and concrete implementation for building order parts
- **Responsibility**: Knows how to create individual components of an order
- **Files**: `OrderBuilder.h`, `ConcreteOrderBuilder.h`, `ConcreteOrderBuilder.cpp`

### 3. Director (OrderDirector)
- **Role**: Controls the construction process using the Builder
- **Responsibility**: Encapsulates the step-by-step construction logic
- **Files**: `OrderDirector.h`, `OrderDirector.cpp`

### 4. Client (Customer)
- **Role**: Initiates the construction process
- **Responsibility**: Calls the Director's construct methods without managing building details
- **Files**: `Customer.h`, `Customer.cpp`

## Class Relationships

```
Customer ----uses----> OrderDirector ----uses----> OrderBuilder
                                                        ^
                                                        |
                                                 ConcreteOrderBuilder
                                                        |
                                                    creates
                                                        v
                                                     Order
```

## Key Methods and Interactions

### Customer Interface
The Customer now provides a clean, high-level interface for order construction:

```cpp
// Main construction method - delegates to Director
Order* construct();

// Specific order types
Order* constructSimplePlantOrder(const string& plantType, int quantity);
Order* constructPlantWithPotOrder(const string& plantType, const string& potType, int quantity);
Order* constructBundleOrder(const string& bundleName, const vector<string>& plantTypes, 
                           const vector<int>& quantities, double discount);

// Predefined order templates
Order* constructGardenStarterKit();
Order* constructSucculentCollection();
```

### Director Methods
The OrderDirector encapsulates all construction logic:

```cpp
class OrderDirector {
public:
    // Main construction entry point
    Order* construct();
    
    // Specific construction algorithms
    Order* constructSimplePlantOrder(const string& plantType, int quantity);
    Order* constructPlantWithPotOrder(const string& plantType, const string& potType, int quantity);
    Order* constructBundleOrder(const string& bundleName, const vector<string>& plantTypes,
                               const vector<int>& quantities, double discount);
    
    // Predefined construction templates
    Order* constructGardenStarterKit();
    Order* constructSucculentCollection();

private:
    OrderBuilder* builder;
    bool validateBuilder() const;
    void resetBuilder();
};
```

## Construction Flow

### 1. Simple Plant Order Flow
```
Customer::constructSimplePlantOrder("Rose", 3)
    |
    v
OrderDirector::constructSimplePlantOrder("Rose", 3)
    |
    v
1. validateBuilder()
2. resetBuilder()
3. builder->buildPlant("Rose", 3)
4. return builder->getOrder()
    |
    v
Order* returned to Customer
```

### 2. Garden Starter Kit Flow
```
Customer::constructGardenStarterKit()
    |
    v
OrderDirector::constructGardenStarterKit()
    |
    v
1. validateBuilder()
2. resetBuilder()
3. buildCustomBundle("Garden Starter Kit", "Beginner Package", 15.0)
4. addPlantToCurrentBundle("Aloe Vera", 1, "small")
5. addPlantToCurrentBundle("Snake Plant", 1, "medium")
6. addPlantToCurrentBundle("Pothos", 1, "small")
7. buildPlantPot("Ceramic", 2)
8. buildPlantPot("Terracotta", 1)
9. return builder->getOrder()
    |
    v
Order* returned to Customer
```

## Benefits of This Implementation

### 1. Clean Customer Code
The Customer no longer needs to:
- Know the specific building steps
- Manage the Builder directly
- Handle complex construction logic
- Worry about the order of operations

**Before (without Director):**
```cpp
// Customer had to manage all building steps
orderBuilder->reset();
orderBuilder->buildPlant("Aloe Vera", 1);
orderBuilder->buildPlant("Snake Plant", 1);
orderBuilder->buildPlant("Pothos", 1);
orderBuilder->buildPlantPot("Ceramic", 2);
orderBuilder->buildPlantPot("Terracotta", 1);
Order* order = orderBuilder->getOrder();
```

**After (with Director):**
```cpp
// Customer just calls one method
Order* order = customer->constructGardenStarterKit();
```

### 2. Encapsulated Construction Logic
All complex building algorithms are contained within the Director:
- Step-by-step construction sequences
- Validation logic
- Error handling
- Builder management

### 3. Flexible and Reusable
- Multiple Directors can use the same Builder
- New construction algorithms can be added to Director without changing Customer
- Predefined templates for common order types
- Easy to extend with new order configurations

### 4. Maintainable
- Changes to construction logic are isolated in the Director
- Customer code remains stable
- Builder interface stays consistent
- Clear separation of concerns

## Usage Examples

### Basic Usage
```cpp
Customer* customer = new Customer("John Doe", "john@email.com");

// Simple one-method construction
Order* order = customer->construct(); // Creates default Garden Starter Kit
```

### Specific Order Types
```cpp
// Simple plant order
Order* roses = customer->constructSimplePlantOrder("Rose", 5);

// Plant with matching pot
Order* cactusSet = customer->constructPlantWithPotOrder("Barrel Cactus", "Ceramic", 2);

// Custom bundle
vector<string> herbs = {"Basil", "Oregano", "Thyme"};
vector<int> quantities = {2, 1, 1};
Order* herbGarden = customer->constructBundleOrder("Herb Collection", herbs, quantities, 20.0);
```

### Predefined Collections
```cpp
// Ready-made collections for different customer needs
Order* starterKit = customer->constructGardenStarterKit();
Order* succulents = customer->constructSucculentCollection();
```

## Error Handling

The Director includes robust error handling:

```cpp
bool OrderDirector::validateBuilder() const {
    if (!builder) {
        cout << "Error: No builder set. Please set a builder before construction." << endl;
        return false;
    }
    return true;
}
```

## Integration with Existing Patterns

### Observer Pattern Integration
The Customer notifies observers about construction events:
```cpp
Order* Customer::construct() {
    notifyInteraction("ORDER_CONSTRUCTION_STARTED", "Customer initiated order construction via Director");
    orderProduct = orderDirector->construct();
    if (orderProduct) {
        notifyInteraction("ORDER_CONSTRUCTED", "Director successfully built order");
    }
    return orderProduct;
}
```

### Command Pattern Integration
The constructed orders can still be executed using the Command pattern:
```cpp
Order* order = customer->construct();
bool success = customer->executeOrder(); // Uses PlaceOrderCommand
```

## Testing and Demonstration

A complete demonstration program (`DirectorPatternDemo.cpp`) shows:
1. All construction methods in action
2. Different order types being built
3. Clean Customer interface usage
4. Pattern benefits in practice

## Conclusion

The Builder-Director Pattern implementation provides:
- **Simplicity**: Customer code is clean and focused
- **Flexibility**: Multiple construction algorithms available
- **Maintainability**: Construction logic is centralized
- **Extensibility**: Easy to add new order types
- **Reusability**: Components can be reused in different contexts

The pattern successfully separates the "what to build" (Customer's choice) from the "how to build" (Director's responsibility), creating a robust and maintainable order construction system.