# COS 214 Project - Greenhouse Management System

## Overview
This project implements a comprehensive greenhouse management system using multiple design patterns including Observer, State, Command, Chain of Responsibility, Composite, Builder, Singleton, Visitor, Bridge, and Strategy patterns.

## Building and Running Tests

### Prerequisites
- g++ compiler with C++11 support
- GNU Make
- Linux/Unix environment

### Available Test Programs

#### 1. Main Greenhouse Program
The main greenhouse management system.

```bash
make greenhouse
./greenhouse
```

#### 2. Move to Sales Floor Test (Autonomous Mode)
Tests the autonomous functionality of moving plants from the greenhouse to the sales floor when they reach the "Ready for Sale" state.

**Features tested:**
- Observer pattern: LifeCycleMonitor notifies StaffManager
- State pattern: ReadyForSaleState triggers automatic movement
- Command pattern: MoveToSalesFloorCommand execution
- Inventory management: Plants move from greenhouse to sales floor

**How to run:**
```bash
make test_move_to_sales_floor
./test_move_to_sales_floor
```

**What to expect:**
- Creates a plant and transitions it through states
- Automatically notifies staff when plant is ready for sale
- Moves plant to sales floor inventory
- Displays before/after inventory counts

#### 3. Customer Order Test (Interactive Mode)
Interactive test simulating the complete customer ordering process with real-time inventory management.

**Features tested:**
- Chain of Responsibility: OrderValidationHandler → PaymentProcessHandler → NotificationHandler
- Composite pattern: SinglePlant and PlantBundle order items
- Builder pattern: ConcreteOrderBuilder for complex orders
- Inventory tracking: Sold plants removed from sales floor and tracked separately
- Detailed validation messages and error handling

**How to run:**
```bash
make test_customer_order
./test_customer_order
```

**Interactive Menu Options:**
1. **Add Single Plant to Order** - Add individual plants with optional pots
2. **Create Plant Bundle** - Create bundles of multiple plants with discount
3. **Place Order** - Process order through validation, payment, and notification
4. **View Current Order** - See order summary with total amount
5. **View Inventory** - Check available plants and sold plants count
6. **Clear Order** - Start a new order
0. **Exit** - Close the program

**Example Workflow:**
```bash
# 1. View initial inventory
Choose option: 5

# 2. Add some plants to order
Choose option: 1
Enter plant type: Rose
Enter quantity: 2
Add pot? (y/n): n

# 3. View order summary
Choose option: 4

# 4. Place order (validates inventory, processes payment, updates inventory)
Choose option: 3

# 5. Check updated inventory (sold plants removed from sales floor)
Choose option: 5
```

**Validation Features:**
- Checks if plant type exists in inventory
- Verifies sufficient quantity available
- Provides detailed error messages for validation failures
- Prevents duplicate sales (sold plants tracked separately)

**Payment Processing:**
- Simulates payment gateway (90% success rate)
- On success: removes plants from sales floor, adds to sold inventory
- On failure: order status updated, inventory unchanged

### Clean Build
Remove all compiled files and start fresh:

```bash
make clean
```

### Rebuild Everything
Clean and rebuild all targets:

```bash
make clean
make greenhouse
make test_move_to_sales_floor
make test_customer_order
```

## Project Structure

```
.
├── Main Classes
│   ├── PlantProduct.*          # Plant subject in Observer pattern
│   ├── LifeCycleMonitor.*      # Observer subject for plant lifecycle
│   ├── StaffManager.*          # Observer for lifecycle events
│   ├── InventoryManager.*      # Singleton managing plant inventory
│   └── Customer.*              # Customer class for orders
│
├── Commands
│   ├── Command.*               # Base command class
│   ├── CareCommand.*           # Plant care commands
│   ├── MoveToSalesFloorCommand.* # Command to move plants to sales floor
│   └── PlaceOrderCommand.*     # Command to place customer orders
│
├── Chain of Responsibility
│   ├── StaffChainHandler.h     # Base handler for staff chain
│   ├── Cashier.h               # Cashier handler
│   ├── OrderProcessHandler.h   # Base handler for order processing
│   ├── OrderValidationHandler.h # Validates orders against inventory
│   ├── PaymentProcessHandler.h  # Processes payment and updates inventory
│   └── NotificationHandler.h    # Sends success/failure notifications
│
├── Order Management (Composite Pattern)
│   ├── Order.*                 # Order aggregate
│   ├── OrderItem.*             # Base component
│   ├── SinglePlant.*           # Leaf - individual plant
│   ├── PlantBundle.*           # Composite - bundle of plants
│   ├── OrderBuilder.h          # Builder interface
│   └── ConcreteOrderBuilder.*  # Concrete builder implementation
│
├── State Pattern
│   ├── StateDP/PlantState.h            # Base state
│   ├── StateDP/PlantedState.h          # Initial state
│   ├── StateDP/InNurseryState.h        # Growing state
│   ├── StateDP/ReadyForSaleState.*     # Triggers move to sales floor
│   └── StateDP/WitheringState.*        # End state
│
├── Bridge Pattern
│   ├── BridgeDP/PlantSpeciesProfile.h  # Abstraction
│   ├── BridgeDP/CactusProfile.h
│   ├── BridgeDP/FlowerProfile.h
│   ├── BridgeDP/SucculentProfile.h
│   └── BridgeDP/TreeProfile.h
│
├── Strategy Pattern
│   ├── StrategyDP/CareStrategy.h
│   ├── StrategyDP/WateringStrategy.h
│   ├── StrategyDP/FertilizingStrategy.h
│   └── StrategyDP/ConcreteStrategies.h
│
├── Visitor Pattern
│   ├── ModeVisitor.h           # Visitor interface
│   ├── AutonomousMode.*        # Autonomous operation mode
│   └── InteractiveMode.*       # Interactive operation mode
│
└── Tests
    ├── MoveToSaleFloorTest.cpp     # Autonomous plant movement test
    ├── CustomerOrderTest.cpp        # Interactive customer ordering test
    └── TestingMain.cpp              # General testing

```

## Design Patterns Used

1. **Observer Pattern**: LifeCycleMonitor (Subject) notifies StaffManager (Observer) of plant state changes
2. **State Pattern**: Plants transition through states (Planted → InNursery → ReadyForSale → Withering)
3. **Command Pattern**: Encapsulates requests (MoveToSalesFloor, PlaceOrder, Care commands)
4. **Chain of Responsibility**: Order processing chain (Validation → Payment → Notification)
5. **Composite Pattern**: Order items (SinglePlant as leaf, PlantBundle as composite)
6. **Builder Pattern**: ConcreteOrderBuilder constructs complex orders
7. **Singleton Pattern**: InventoryManager ensures single instance
8. **Visitor Pattern**: AutonomousMode and InteractiveMode operations
9. **Bridge Pattern**: Plant species profiles
10. **Strategy Pattern**: Care strategies (watering, fertilizing, pruning)

## Key Features

### Inventory Management
- **Greenhouse Inventory**: Plants in growing phase
- **Sales Floor Inventory**: Plants ready for sale
- **Sold Plants Tracking**: Separate vector for sold plants
- **Automatic Updates**: Sold plants removed from sales floor during payment processing

### Order Processing Chain
1. **Validation**: Checks inventory availability with detailed error messages
2. **Payment**: Processes payment and updates inventory on success
3. **Notification**: Sends success or failure notification to customer

### Error Handling
- Detailed validation error messages (e.g., "Rose is currently out of stock")
- Payment failure handling with appropriate status updates
- Inventory conflict prevention (sold plants can't be sold again)

## Notes

- The payment processor has a 90% success rate to simulate real-world payment failures
- All sold plants are tracked separately and won't appear in available inventory
- The system uses observer pattern for autonomous plant movement when ready for sale
- Coverage flags are enabled for code coverage analysis (--coverage)

## Authors
COS 214 Project Team

## License
Academic Project - University of Pretoria
