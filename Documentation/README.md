# Greenhouse Management System

A comprehensive C++ application simulating a greenhouse management system, designed to demonstrate the practical application of various software design patterns in a modular and scalable architecture.

## Core Features

*   **Plant Lifecycle Management**: Plants automatically transition through states (`Planted`, `InNursery`, `Growing`, `ReadyForSale`) based on time and care events.
*   **Staff Coordination System**: Utilizes the **Chain of Responsibility** pattern to delegate tasks (watering, pruning, sales) to the appropriate staff members, ensuring tasks are handled even if a specific staff member is busy.
*   **Dynamic Care Strategies**: Implements the **Strategy** pattern to apply different care algorithms (e.g., Drip Watering, Standard Pruning) based on plant type.
*   **Flexible Plant & Product System**:
    *   Uses the **Bridge** pattern to separate the `PlantProduct` abstraction from its specific species implementation (`FlowerProfile`, `SucculentProfile`).
    *   Leverages the **Factory** pattern for creating families of related objects like pots (`ClayPot`, `PlasticPot`).
    *   Employs the **Decorator** pattern to dynamically add features (e.g., color, texture) to products like pots.
*   **Event-Driven Notifications**: Employs the **Observer** pattern to notify manager systems of important plant lifecycle events, enabling a decoupled architecture.
*   **Order & Payment Processing**:
    *   **Builder**: Constructs complex `Order` objects step-by-step.
    *   **Adapter**: Integrates multiple third-party payment systems (Cash, Credit Card, EFT) through a unified interface.
    *   **Memento**: Captures and restores the state of an `Order` without violating encapsulation, allowing for undo/rollback functionality.
*   **Automated Build System**: A comprehensive `Makefile` for easy compilation, testing, and cleaning of the project.
*   **Automated CI/CD Pipeline**: A GitHub Actions workflow that automatically lints, builds, and runs unit tests on every push to ensure code quality and stability.

## Design Patterns Implemented

This project serves as a practical portfolio of the following design patterns:

### Creational Patterns
*   **Factory Method**: Used in `PotFactory` to create different types of pots.
*   **Builder**: Used to construct complex `Order` objects.
*   **Singleton**: Ensures a single instance of manager classes like `InventoryManager`.

### Structural Patterns
*   **Adapter**: Integrates incompatible payment systems.
*   **Bridge**: Decouples `PlantProduct` from its `PlantSpeciesProfile`.
*   **Composite**: Manages a tree structure of greenhouse components.
*   **Decorator**: Adds features like color and texture to pots.
*   **Facade**: Provides a simplified interface for complex subsystems like order processing.

### Behavioral Patterns
*   **Chain of Responsibility**: Manages task delegation among staff members.
*   **Command**: Encapsulates requests as objects (e.g., `WaterCommand`, `PruneCommand`).
*   **Memento**: Saves and restores the state of an `Order`.
*   **Observer**: Notifies observers of plant lifecycle events.
*   **State**: Allows a `PlantProduct` to alter its behavior when its internal state changes.
*   **Strategy**: Defines a family of algorithms for plant care and allows them to be interchanged.
*   **Template Method**: Defines the skeleton of an algorithm for generating bouquet suggestions, deferring specific steps to subclasses.

## Getting Started

### Prerequisites
*   `g++` (with C++11 support)
*   `make`
*   `pkg-config`
*   `gtk+-3.0` (for GUI components)
*   `valgrind` (for memory analysis)

### Build
To compile the main application and all test executables, run:
```bash
make
```

### Run the Main Application
```bash
./greenhouse
```

## Running Tests

The project includes a suite of unit tests to ensure functionality and stability.

### Run All Tests
To build and run all automated tests:
```bash
make test
```

### Run a Specific Test
To run a single test executable (e.g., `staff_test`):
```bash
make run-test TEST=staff_test
```

### Memory Analysis
To run `valgrind` on the main application and all tests to check for memory leaks:
```bash
make valgrind
```

## CI/CD Pipeline

This repository is equipped with a GitHub Actions CI/CD pipeline (`.github/workflows/main.yml`) that performs the following checks on every push:
1.  **Linting**: Checks for style and syntax errors using `cppcheck` and `clang-format`.
2.  **Build**: Compiles the entire project to ensure there are no compilation errors.
3.  **Test**: Runs the complete test suite using the `make test` command.

## Test Coverage

The project includes comprehensive unit tests for all major components:

*   **Observer Pattern Tests** (`Observer_test.cpp`): Tests observer registration, notifications, and removal.
*   **Builder Pattern Tests** (`builder_test.cpp`): Tests greenhouse structure construction and plant management.
*   **Plant System Tests** (`plant_system_test.cpp`): Tests state transitions, bridge pattern, strategy pattern, and plant profiles.
*   **Staff Coordination Tests** (`staff_test.cpp`): Tests chain of responsibility, command handling, and delegation (33 test assertions).
*   **Factory Pattern Tests** (`PotFactory_testing_main.cpp`): Tests pot creation using different factory implementations.

**Test Results**: All 5 test suites pass with 100% success rate.

## Project Structure

```
COS214-P5/
├── .github/
│   └── workflows/
│       └── main.yml          # CI/CD pipeline configuration
├── *.cpp / *.h               # Source and header files
├── *_test.cpp                # Unit test files
├── Makefile                  # Build automation
├── main.cpp                  # Application entry point
└── README.md                 # This file
```

## Contributing

This is an academic project demonstrating design pattern implementation. Contributions are not currently accepted, but feel free to fork and experiment.

## Documentation

For detailed documentation on:
*   **CI/CD Setup**: See `CI_CD_GUIDE.md` and `CI_CD_QUICKSTART.md`
*   **Testing**: See `TESTING_README.md`
*   **Project Setup**: See `SETUP_GUIDE.md`

