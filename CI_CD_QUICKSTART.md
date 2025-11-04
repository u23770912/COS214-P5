# CI/CD Pipeline - Quick Start

1. **🔍 Lints your code** - Checks for errors and style issues
2. **🔨 Builds the application** - Compiles your C++ code
3. **🧪 Runs tests** - Executes automated tests
4. **📊 Analyzes memory** - Checks for memory leaks with Valgrind

## 📁 Files Created/Modified

- `.github/workflows/main.yml` - GitHub Actions workflow (CI/CD pipeline)
- `Makefile` - Enhanced with test and valgrind targets
- `CI_CD_GUIDE.md` - Complete documentation

## 🚀 How to Use It

### Automatic (GitHub Actions)

**Every time you push code:**
```bash
git add .
git commit -m "Your changes"
git push
```

GitHub will automatically:
1. Run the pipeline (see "Actions" tab)
2. Show ✅ or ❌ status
3. Email you if something fails

### Manual (Local Testing)

Before pushing, test locally:

```bash
# 1. Build the project
make clean && make

# 2. Run tests
make test

# 3. Check for memory leaks
make valgrind

# 4. See available commands
make help
```

## 📊 Viewing Results

### On GitHub:
1. Go to your repository: https://github.com/u23770912/COS214-P5
2. Click **"Actions"** tab at the top
3. See all workflow runs with status
4. Click any run to see detailed logs

### Status Indicators:
- ✅ **Green** = All checks passed
- ❌ **Red** = Something failed (click to see what)
- 🟡 **Yellow** = Currently running

## 🎯 Pipeline Jobs

Your pipeline has 5 jobs that run in sequence:

```
1. LINT    → 2. BUILD → 3. TEST → 4. COVERAGE → 5. SUCCESS
   ↓            ↓          ↓           ↓            ↓
Checks       Compiles   Runs      Measures    Confirms
code         C++        tests     coverage    all passed
quality
```

### Job Details:

**1. Lint (🔍 Code Quality)**
- Runs `cppcheck` for static analysis
- Checks code style with `clang-format`
- Validates syntax of all .cpp/.h files

**2. Build (🔨 Compilation)**
- Installs dependencies (GTK3, etc.)
- Compiles with `make -j` (parallel)
- Caches build artifacts for speed
- Uploads executable for testing

**3. Test (🧪 Validation)**
- Downloads compiled executable
- Runs `make test`
- Executes Valgrind memory check
- Generates test report

**4. Coverage (📊 Analysis)**
- Measures code coverage (on PRs only)
- Shows which code is tested

**5. Success (✅ Confirmation)**
- Shows summary of all results
- Confirms pipeline completed

## 🔧 Local Commands

```bash
# Build
make              # Compile the project
make clean        # Remove build artifacts

# Test
make test         # Run automated tests
make unit-test    # Run unit tests (when implemented)

# Analysis
make valgrind     # Check for memory leaks

# Help
make help         # Show all available commands
```

## 📝 Adding Unit Tests

Currently `make test` just runs your app. To add real unit tests:

### Quick Start (Simple Tests):

**1. Create `tests/simple_test.cpp`:**
```cpp
#include <iostream>
#include <cassert>

void test_example() {
    assert(1 + 1 == 2);
    std::cout << "✓ Math works!\n";
}

int main() {
    std::cout << "Running tests...\n";
    test_example();
    std::cout << "✅ All tests passed!\n";
    return 0;
}
```

**2. Update Makefile test target:**
```makefile
test: tests/simple_test
	./tests/simple_test

tests/simple_test: tests/simple_test.cpp
	$(CXX) $(CXXFLAGS) $< -o $@
```

### Advanced (Google Test):

See `CI_CD_GUIDE.md` for complete Google Test setup instructions.

## 🛠️ Troubleshooting

### Pipeline fails but code works locally?

**Check:**
1. Dependencies installed in CI (see workflow file)
2. GTK3 might need headless mode: `xvfb-run ./greenhouse`
3. Paths are correct (relative paths work better)

### Tests timeout?

Add timeout to workflow:
```yaml
timeout-minutes: 5
```

### Want to skip CI on a commit?

Add `[skip ci]` to commit message:
```bash
git commit -m "Update docs [skip ci]"
```


### Enable Branch Protection

1. Go to Settings → Branches
2. Add rule for `main` branch
3. Enable "Require status checks to pass"
4. Select your workflow
5. Now PRs can't merge if tests fail!

