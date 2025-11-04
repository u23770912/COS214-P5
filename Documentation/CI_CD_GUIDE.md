# CI/CD Pipeline Guide

## Table of Contents
1. [What is CI/CD?](#what-is-cicd)
2. [Pipeline Overview](#pipeline-overview)
3. [How It Works](#how-it-works)
4. [Jobs Explained](#jobs-explained)
5. [Testing Locally](#testing-locally)
6. [Viewing Results](#viewing-results)
7. [Adding Unit Tests](#adding-unit-tests)

---

## What is CI/CD?

### Continuous Integration (CI)
- **Automatically builds** your code when you push changes
- **Runs tests** to catch bugs early
- **Checks code quality** with linters and static analysis
- **Prevents broken code** from being merged

### Continuous Deployment (CD)
- **Automatically deploys** your application after tests pass
- **Reduces manual work** and human error
- **Speeds up release cycles**

### Why Use It?
**Catch bugs early** - Before they reach production  
**Maintain quality** - Automated checks on every commit  
**Team confidence** - Know changes won't break existing features  
**Documentation** - See what tests run and results  
**Save time** - No manual testing on every change  

---

## Pipeline Overview

```
┌─────────────────────────────────────────────────────────────────┐
│                     GITHUB PUSH/PULL REQUEST                     │
└────────────────────────────┬────────────────────────────────────┘
                             │
                             ▼
                   ┌─────────────────┐
                   │  TRIGGER WORKFLOW │
                   └─────────┬─────────┘
                             │
        ┌────────────────────┼────────────────────┐
        │                    │                    │
    ┌───▼───┐          ┌─────▼─────┐       ┌─────▼─────┐
    │ LINT  │          │   BUILD   │       │   TEST    │
    │  JOB  │─────────▶│    JOB    │──────▶│    JOB    │
    └───┬───┘          └─────┬─────┘       └─────┬─────┘
        │                    │                    │
        │                    │                    │
    ┌───▼────────┐      ┌────▼─────────┐    ┌────▼──────────┐
    │ • cppcheck │      │ • Compile    │    │ • Run tests   │
    │ • syntax   │      │ • Link       │    │ • Valgrind    │
    │ • style    │      │ • Artifacts  │    |               │
    └────────────┘      └──────────────┘    └───────┬───────┘
                                                     │
                                                     ▼
                                            ┌────────────────┐
                                            │ ✅ SUCCESS or  │
                                            │ ❌ FAILURE     │
                                            └────────────────┘
```

---

## How It Works

### 1. **Trigger Events**
The pipeline runs when:
- You **push** to: `main`, `Dev-Branch`, `Unit-Test`, `Testing-branch`, or `feature/*` branches
- Someone creates a **pull request** to `main` or `Dev-Branch`

### 2. **Workflow Execution**
```yaml
# This file: .github/workflows/main.yml
name: C++ CI/CD Pipeline
on:
  push:
    branches: [ "main", "Dev-Branch", "Unit-Test" ]
  pull_request:
    branches: [ "main", "Dev-Branch" ]
```

### 3. **Runner Environment**
- **OS**: Ubuntu Latest (Linux)
- **Resources**: 2 CPU cores, 7 GB RAM
- **Free**: 2000 minutes/month for public repos

---

## Jobs Explained

### Job 1: LINT (Code Quality)

**Purpose**: Check code quality before building

**Steps**:
1. **Checkout Code** - Downloads your repository
2. **Install Tools** - Installs `cppcheck` and `clang-format`
3. **Static Analysis** - Finds bugs without running code
   ```bash
   cppcheck --enable=all --std=c++11 .
   ```
4. **Style Check** - Ensures consistent formatting
5. **Syntax Check** - Validates all files compile

**What It Catches**:
- Undefined variables
- Memory leaks
- Unused code
- Logic errors
- Style violations

---

### Job 2: BUILD (Compilation)

**Purpose**: Compile the application

**Steps**:
1. **Install Dependencies** - GTK3, build tools
2. **Cache Build** - Speeds up subsequent builds
3. **Compile Project** - Runs `make` or `cmake`
4. **Upload Artifacts** - Saves executable for testing

**Features**:
- Parallel compilation (`-j` flag)
- Caches object files
- Stores build artifacts (7 days)

---

### Job 3: TEST (Validation)

**Purpose**: Run automated tests

**Steps**:
1. **Download Artifacts** - Gets compiled executable
2. **Run Unit Tests** - Executes `make test`
3. **Memory Analysis** - Runs Valgrind
4. **Generate Report** - Creates test summary

**Tests Run**:
- Unit tests (when implemented)
- Integration tests
- Memory leak detection
- Performance checks

---


### Job 4: SUCCESS

**Purpose**: Confirmation message

Shows summary:
```
🎉 CI/CD PIPELINE COMPLETED 🎉
✓ Lint: Passed
✓ Build: Passed
✓ Test: Passed
```

---

## Testing Locally

Before pushing, test the pipeline locally:

### 1. **Run Lint Checks**
```bash
# Install cppcheck
sudo apt-get install cppcheck

# Run static analysis
cppcheck --enable=all --std=c++11 --force .
```

### 2. **Build Project**
```bash
# Clean and build
make clean
make

# Should output:
# Compilation successful
# Created executable: greenhouse
```

### 3. **Run Tests**
```bash
# Run test suite
make test

# Should show:
# ✓ Executable found
# ✓ Tests completed
```

### 4. **Check Memory Leaks**
```bash
# Run Valgrind
make valgrind

# Check report:
# ✓ No leaks detected
# ⚠ Warnings if leaks found
```

---

## Viewing Results

### On GitHub:

1. **Go to your repository**
2. **Click "Actions" tab**
3. **See workflow runs**

### Workflow Status:
- ✅ **Green checkmark** = All passed
- ❌ **Red X** = Something failed
- 🟡 **Yellow dot** = Running

### Detailed Logs:
1. Click on a workflow run
2. Click on a job (Lint/Build/Test)
3. Expand steps to see logs
4. Download artifacts if available

### Status Badges:
Add to README.md:
```markdown
![CI/CD Pipeline](https://github.com/u23770912/COS214-P5/actions/workflows/main.yml/badge.svg)
```

---

## Adding Unit Tests

Currently your `make test` runs the application. Here's how to add real unit tests:

### Option 1: Google Test (Recommended)

**1. Install Google Test**
```bash
# Ubuntu/Debian
sudo apt-get install libgtest-dev

# Build Google Test
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
sudo cp *.a /usr/lib
```

**2. Create Test File**
```cpp
// test_greenhouse.cpp
#include <gtest/gtest.h>
#include "PlantProduct.h"
#include "InventoryManager.h"

TEST(InventoryTest, AddPlant) {
    InventoryManager* inv = InventoryManager::getInstance();
    // Add test logic
    EXPECT_NE(inv, nullptr);
}

TEST(PlantTest, StateTransition) {
    // Test plant state changes
    EXPECT_EQ(1, 1);  // Example assertion
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```

**3. Update Makefile**
```makefile
# Add test executable
TEST_TARGET = greenhouse_test
TEST_SRCS = test_greenhouse.cpp
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

$(TEST_TARGET): $(TEST_OBJS) $(filter-out main.o, $(OBJS))
	$(CXX) -o $@ $^ $(LDFLAGS) -lgtest -lgtest_main -pthread

test: $(TEST_TARGET)
	./$(TEST_TARGET)
```

### Option 2: Simple Custom Tests

**1. Create simple_test.cpp**
```cpp
#include <iostream>
#include <cassert>
#include "InventoryManager.h"

void test_inventory_singleton() {
    auto inv1 = InventoryManager::getInstance();
    auto inv2 = InventoryManager::getInstance();
    assert(inv1 == inv2);
    std::cout << "✓ Singleton test passed\n";
}

void test_plant_creation() {
    // Add your test logic
    std::cout << "✓ Plant creation test passed\n";
}

int main() {
    std::cout << "Running tests...\n";
    test_inventory_singleton();
    test_plant_creation();
    std::cout << "All tests passed!\n";
    return 0;
}
```

**2. Update Makefile**
```makefile
test: simple_test
	./simple_test

simple_test: simple_test.cpp $(filter-out main.o, $(OBJS))
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
```

---

## Advanced Features

### Enable Code Coverage

**1. Update Makefile**
```makefile
CXXFLAGS += --coverage  # Add coverage flags
LDFLAGS += --coverage

coverage: test
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory out
	@echo "Coverage report: out/index.html"
```

### Add Docker Support

**Create Dockerfile**:
```dockerfile
FROM ubuntu:22.04
RUN apt-get update && apt-get install -y build-essential
WORKDIR /app
COPY . .
RUN make
CMD ["./greenhouse"]
```

### Matrix Testing (Multiple Versions)

```yaml
strategy:
  matrix:
    os: [ubuntu-22.04, ubuntu-20.04]
    compiler: [g++-11, g++-12]
```

---

## Troubleshooting

### Build Fails on GitHub but Works Locally

**Cause**: Missing dependencies in CI environment

**Fix**: Add to workflow:
```yaml
- name: Install Dependencies
  run: |
    sudo apt-get install -y libgtk-3-dev
```

### Tests Timeout

**Cause**: Tests run too long

**Fix**: Add timeout:
```yaml
- name: Run Tests
  timeout-minutes: 5
  run: make test
```

### GTK Warning in CI

**Cause**: No display server in CI

**Fix**: Use headless mode:
```bash
xvfb-run ./greenhouse
```

---


## Resources

- [GitHub Actions Docs](https://docs.github.com/en/actions)
- [Google Test Tutorial](https://google.github.io/googletest/)
- [CPPCheck Manual](http://cppcheck.sourceforge.net/manual.pdf)
- [Valgrind Quick Start](https://valgrind.org/docs/manual/quick-start.html)

---
