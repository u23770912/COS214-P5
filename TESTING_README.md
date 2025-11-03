# Testing with Multiple main() Functions

## ✅ Setup Complete!

Your project now supports **multiple test files with their own main() functions**. Each test compiles into its own executable and runs independently.

## 📋 Current Tests

- ✅ `builder_Testing_main.cpp` - Tests greenhouse builder pattern

## 🚀 Quick Commands

```bash
# List available tests
make list-tests

# Run all tests
make test

# Run specific test
make run-test TEST=builder_Testing_main

# Build tests without running
make build-tests
```

## ➕ Adding New Tests

### Step 1: Create Your Test File

Create a new `.cpp` file with its own `main()` function:

```cpp
// my_new_test.cpp
#include <iostream>
#include <cassert>
#include "YourClasses.h"

int main() {
    std::cout << "=== My New Test ===" << std::endl;
    
    // Your test code here
    assert(1 + 1 == 2);
    
    std::cout << "✅ All tests passed!" << std::endl;
    return 0;  // 0 = success, non-zero = failure
}
```

### Step 2: Register in Makefile

Edit the `Makefile` and add your test to `WORKING_TESTS`:

```makefile
# Test configuration
WORKING_TESTS := builder_Testing_main.cpp
WORKING_TESTS += my_new_test.cpp        # <-- Add your test here
# WORKING_TESTS += another_test.cpp     # <-- Add more tests
```

### Step 3: Run Your Test

```bash
# Build and run
make run-test TEST=my_new_test

# Or run all tests
make test
```

## 📊 Test Output

Your tests will show:
- ✅ PASSED - if return 0
- ❌ FAILED - if return non-zero or timeout

Example output:
```
==========================================
    🧪 Running All Tests
==========================================

▶ Running: builder_Testing_main
----------------------------------------
=== FINAL GREENHOUSE STRUCTURE ===
...test output...
✅ PASSED: builder_Testing_main
----------------------------------------

==========================================
  📊 Test Summary
==========================================
  ✅ Passed: 1
  ❌ Failed: 0
  📝 Total:  1
==========================================
```

## 🤖 CI/CD Integration

Your tests run automatically in GitHub Actions whenever you push code:

1. **Push your code**
   ```bash
   git add .
   git commit -m "Add new tests"
   git push
   ```

2. **Check results** at: https://github.com/u23770912/COS214-P5/actions

3. **Pipeline runs**:
   - ✅ Lint (code quality)
   - ✅ Build (compilation)
   - ✅ Test (runs `make test`)
   - ✅ Valgrind (memory check)

## 💡 Test Guidelines

### Return Codes
- `return 0` = Test passed ✅
- `return 1` (or any non-zero) = Test failed ❌

### Example Test Structure
```cpp
int main() {
    int passed = 0, failed = 0;
    
    try {
        // Test 1
        assert(condition);
        std::cout << "✓ Test 1 passed\n";
        passed++;
    } catch (...) {
        std::cout << "✗ Test 1 failed\n";
        failed++;
    }
    
    // More tests...
    
    std::cout << "Summary: " << passed << " passed, " 
              << failed << " failed\n";
    return (failed == 0) ? 0 : 1;
}
```

### Timeout
- Tests automatically timeout after 10 seconds
- Adjust in Makefile if needed: `timeout 30s`

## 🔧 Troubleshooting

### Test not found
```bash
make list-tests  # Check if test is registered
```

### Compilation errors
- Make sure all required `.cpp` files exist
- Check includes are correct
- Ensure test is added to `WORKING_TESTS` in Makefile

### Test fails in CI but works locally
- Check dependencies are installed in CI
- Review GitHub Actions logs
- Test may need headless mode for GUI components

## 📚 More Info

- Full testing guide: `TESTING_GUIDE.md`
- CI/CD documentation: `CI_CD_GUIDE.md`
- CI/CD quickstart: `CI_CD_QUICKSTART.md`

---

**🎉 Your testing infrastructure is ready!**

Just create test files, add them to the Makefile, and run `make test`!
