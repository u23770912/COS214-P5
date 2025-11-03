# Compiler and flags
CXX = g++
CXXFLAGS = -g -std=c++11 -Wall -I. `pkg-config --cflags gtk+-3.0`
LDFLAGS = `pkg-config --libs gtk+-3.0`

# Source files
SRCS := $(wildcard *.cpp)
TEST_FILES := builder_Testing_main.cpp, test_madt.cpp
SRCS := $(filter-out main.cpp $(TEST_FILES), $(SRCS))
OBJS := $(SRCS:.cpp=.o)

# Main application
TARGET = greenhouse
MAIN_OBJS = $(OBJS) main.o

# Test configuration
WORKING_TESTS := builder_Testing_main.cpp, test_madt.cpp
TEST_TARGETS := $(WORKING_TESTS:.cpp=)

.PHONY: all clean test valgrind help unit-test run-tests clean-tests build-tests list-tests run-test

all: $(TARGET)

# Build main application
$(TARGET): $(MAIN_OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compile .cpp files to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build individual test executables
$(TEST_TARGETS): %: %.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) $< $(OBJS) -o $@ $(LDFLAGS)

# Build all test executables
build-tests: $(TEST_TARGETS)

# Run all tests
run-tests: build-tests
	@PASS=0; FAIL=0; \
	for test in $(TEST_TARGETS); do \
		echo "Running: $$test"; \
		if timeout 10s ./$$test; then \
			echo "PASSED: $$test"; \
			PASS=$$((PASS + 1)); \
		else \
			echo "FAILED: $$test"; \
			FAIL=$$((FAIL + 1)); \
		fi; \
		echo ""; \
	done; \
	echo "Test Summary:"; \
	echo "  Passed: $$PASS"; \
	echo "  Failed: $$FAIL"; \
	echo "  Total:  $$((PASS + FAIL))"; \
	if [ $$FAIL -gt 0 ]; then exit 1; fi

test: run-tests

unit-test: run-tests

# Run a specific test
run-test: 
	@if [ -z "$(TEST)" ]; then \
		echo "ERROR: Please specify TEST=<test_name>"; \
		echo "Available tests:"; \
		for test in $(TEST_TARGETS); do echo "  - $$test"; done; \
		exit 1; \
	fi
	@if [ ! -f "$(TEST)" ]; then \
		$(MAKE) $(TEST); \
	fi
	@./$(TEST)

# List all available tests
list-tests:
	@echo "Available test executables:"
	@for test in $(TEST_TARGETS); do echo "  - $$test"; done

# Clean test executables
clean-tests:
	@rm -f $(TEST_TARGETS)

# Clean everything
clean: clean-tests
	@rm -f $(OBJS) main.o $(TARGET) *.gcda *.gcno *.gcov coverage.info valgrind-report.txt
	@rm -rf out

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
		--verbose --log-file=valgrind-report.txt ./$(TARGET)

# Help target
help:
	@echo "Available targets:"
	@echo "  make              - Build main application"
	@echo "  make all          - Build main application"
	@echo "  make build-tests  - Build all test executables"
	@echo "  make test         - Build and run all tests"
	@echo "  make run-tests    - Run all tests"
	@echo "  make run-test TEST=<name> - Run specific test"
	@echo "  make list-tests   - Show available tests"
	@echo "  make valgrind     - Run memory leak analysis"
	@echo "  make clean        - Remove all build artifacts"
	@echo "  make clean-tests  - Remove test executables"
	@echo "  make help         - Show this help message"
