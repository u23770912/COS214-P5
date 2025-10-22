# 🌱 Observer Pattern - Plant Lifecycle Monitoring

## Overview

This code implements the **Observer Pattern** to automatically notify interested parties() when a plant's state changes throughout its lifecycle. When a plant transitions between states (PlantedState → PropagationState → ReadyForTransplantState → InNurseryState → ReadyForSaleState → SoldState), all registered observers are notified and can react accordingly.

## 📋 Key Components

### Core Classes

| Class               | Role               | Responsibility                                             |
| ------------------- | ------------------ | ---------------------------------------------------------- |
| `LifeCycleObserver` | Observer Interface | Defines the `update()` method all observers must implement |
| `LifeCycleMonitor`  | Subject Manager    | Manages observer list and handles notifications            |
| `PlantProduct`      | Observable Subject | The plant whose state changes trigger notifications        |
| `StaffManager`      | Concrete Observer  | Dispatches care tasks to staff based on plant state        |

### Supporting Classes

- **`PlantState`** - Multiple States representing lifecycle states (SEED, SPROUTED, GROWING, etc.)
- **`StaffMember`** - Chain of Responsibility handler for care tasks
- **`CareCommand`** - Abstract base for care commands (Water, Fertilize, Prune, etc.)

## 🔄 How It Works

```
Plant State Change
       ↓
setState() called
       ↓
LifeCycleMonitor.notify()
       ↓
    ┌──────────────┬──────────────┐
    ↓              ↓              ↓
StaffManager   InventoryMgr   [Other Observers]
    ↓              ↓
Execute tasks  Update stock
```

### Example Flow

```cpp
// 1. Setup
PlantProduct* rose = new PlantProduct("Rose");
StaffManager* staffMgr = new StaffManager();
InventoryManager* invMgr = new InventoryManager();

// 2. Register observers
rose->registerObserver(staffMgr);
rose->registerObserver(invMgr);

// 3. State changes automatically notify observers
rose->advanceState();  // SEED → SPROUTED
// Output:
// StaffManager: Plant 'Rose' changed to state SPROUTED
//   -> Dispatching to greenhouse staff chain
// InventoryManager: Updating stock for 'Rose'

// 4. Harvest triggers inventory update
rose->dispose();  // → HARVESTED
// InventoryManager adds 1 unit of "Rose" to stock
```

## 🔗 Pattern Integration

### With Command Pattern

Commands execute actions that may change plant state, which triggers observer notifications:

```cpp
WaterCommand* cmd = new WaterCommand(plant);
cmd->execute();  // May advance state → observers notified
```

### With Chain of Responsibility

Observers dispatch commands to staff chains:

```cpp
// Plant state changes
plant->setState(PlantState::GROWING);

// StaffManager receives notification
// → Evaluates state
// → Dispatches CareCommand to appropriate chain
// → First available StaffMember handles it
```

## 📁 File Structure

```
Observer/
├── LifeCycleObserver.h      # Observer interface
├── LifeCycleMonitor.h       # Subject manager
├── PlantProduct.h           # Observable plant
├── PlantState.h             # States
├── StaffManager.h           # Concrete observer
├── StaffMember.h            # Chain of Responsibility
└── CareCommand.h            # Command pattern base
    ├── WaterCommand.h
    ├── FertilizeCommand.h
    ├── PruneCommand.h
    ├── ReportCommand.h
    └── MoveToSalesFloorCommand.h
```

## 🚀 Usage

### Basic Example

```cpp
// Create plant and observers
PlantProduct* plant = new PlantProduct("Tomato");
StaffManager* staffMgr = new StaffManager();
InventoryManager* invMgr = new InventoryManager();

// Register observers
plant->registerObserver(staffMgr);
plant->registerObserver(invMgr);

// Any state change notifies all observers
plant->advanceState();  // Automatic notifications
```

### Complete Lifecycle

```cpp
plant->setState(PlantState::PlantedState);           // Initial planting
plant->setState(PlantState::PropagationState);       // After propagation begins
plant->setState(PlantState::ReadyForTransplantState);// After growth milestone
plant->setState(PlantState::InNurseryState);         // In nursery care
plant->setState(PlantState::ReadyForSaleState);      // Ready for customers
plant->setState(PlantState::SoldState);              // Customer purchased
plant->dispose();                          // Harvested → inventory updated
```

## 📊 State Transitions

```
PlantedState → PropagationState → ReadyForTransplantState → InNurseryState → ReadyForSaleState → SoldState
     ↓                  ↓                   ↓                       ↓               ↓                 ↓
    [Greenhouse Staff]   [Greenhouse Staff]    [Greenhouse Staff]   [Greenhouse Staff]      [Sales Staff]
                                          [Inventory Update]
```

---
