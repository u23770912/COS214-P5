CXX = g++
CXXFLAGS = -g -std=c++11 -Wall --coverage

# Source files
SRC = $(wildcard *.cpp) $(wildcard StateDP/*.cpp) $(wildcard BridgeDP/*.cpp) $(wildcard StrategyDP/*.cpp) $(wildcard SuggestionTemplate/*.cpp)
DEPS = $(wildcard *.h) $(wildcard StateDP/*.h) $(wildcard BridgeDP/*.h) $(wildcard StrategyDP/*.h) $(wildcard SuggestionTemplate/*.h)

# Object files - use proper path handling
OBJ = $(patsubst %.cpp,%.o,$(SRC))

# Exclude test files from main
MAIN_SRC = $(filter-out MoveToSaleFloorTest.cpp CustomerOrderTest.cpp test_memento_adapter.cpp DemoMain.cpp,$(SRC))
MAIN_OBJ = $(patsubst %.cpp,%.o,$(MAIN_SRC))

# Demo main (exclude all other mains and tests)
DEMO_SRC = $(filter-out main.cpp MoveToSaleFloorTest.cpp CustomerOrderTest.cpp test_memento_adapter.cpp,$(SRC))
DEMO_OBJ = $(patsubst %.cpp,%.o,$(DEMO_SRC))

# Test files - MoveToSalesFloor test (exclude main.cpp, InteractiveMode.cpp, DemoMain.cpp)
MOVE_TEST_SRC = $(filter-out main.cpp InteractiveMode.cpp CustomerOrderTest.cpp test_memento_adapter.cpp DemoMain.cpp,$(SRC))
MOVE_TEST_OBJ = $(patsubst %.cpp,%.o,$(MOVE_TEST_SRC))

# Customer Order test (exclude main.cpp, MoveToSaleFloorTest.cpp, DemoMain.cpp)
ORDER_TEST_SRC = $(filter-out main.cpp MoveToSaleFloorTest.cpp test_memento_adapter.cpp DemoMain.cpp,$(SRC))
ORDER_TEST_OBJ = $(patsubst %.cpp,%.o,$(ORDER_TEST_SRC))

# Memento & Adapter test (exclude main.cpp, DemoMain.cpp and other test files)
test_memento_adapter_SRC = $(filter-out main.cpp MoveToSaleFloorTest.cpp CustomerOrderTest.cpp DemoMain.cpp,$(SRC))
test_memento_adapter_OBJ = $(patsubst %.cpp,%.o,$(test_memento_adapter_SRC))

TARGET = greenhouse
DEMO_TARGET = demo
TEST_MOVE_TARGET = test_move_to_sales_floor
TEST_ORDER_TARGET = test_customer_order
TEST_MEMENTO_ADAPTER_TARGET = test_memento_adapter

all: $(TARGET) $(DEMO_TARGET)

test: $(TEST_MOVE_TARGET) $(TEST_ORDER_TARGET) $(TEST_MEMENTO_ADAPTER_TARGET)

$(TARGET): $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(DEMO_TARGET): $(DEMO_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_MOVE_TARGET): $(MOVE_TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_ORDER_TARGET): $(ORDER_TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_MEMENTO_ADAPTER_TARGET): $(test_memento_adapter_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) $(DEMO_TARGET) $(TEST_MOVE_TARGET) $(TEST_ORDER_TARGET) $(TEST_MEMENTO_ADAPTER_TARGET)
	rm -f *.gcda *.gcno *.gcov coverage.info StateDP/*.gcda StateDP/*.gcno BridgeDP/*.gcda BridgeDP/*.gcno StrategyDP/*.gcda StrategyDP/*.gcno
	rm -rf out

run-demo: $(DEMO_TARGET)
	./$(DEMO_TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(TARGET)

valgrind-demo: $(DEMO_TARGET)
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(DEMO_TARGET)

valgrind-memento: $(TEST_MEMENTO_ADAPTER_TARGET)
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(TEST_MEMENTO_ADAPTER_TARGET)

coverage: all
	./$(TARGET)
	@echo "Generating coverage report..."
	@mkdir -p out
	@lcov --capture --directory . --output-file coverage.info
	@genhtml coverage.info --output-directory out
	@echo "Coverage report generated in out/index.html"

coverage-demo: $(DEMO_TARGET)
	./$(DEMO_TARGET)
	@echo "Generating coverage report for demo..."
	@mkdir -p out
	@lcov --capture --directory . --output-file coverage.info
	@genhtml coverage.info --output-directory out
	@echo "Coverage report generated in out/index.html"

coverage-memento: $(TEST_MEMENTO_ADAPTER_TARGET)
	./$(TEST_MEMENTO_ADAPTER_TARGET)
	@echo "Generating coverage report for Memento & Adapter test..."
	@mkdir -p out
	@lcov --capture --directory . --output-file coverage.info
	@genhtml coverage.info --output-directory out
	@echo "Coverage report generated in out/index.html"

coverage-clean:
	rm -f *.gcda *.gcno *.gcov coverage.info StateDP/*.gcda StateDP/*.gcno BridgeDP/*.gcda BridgeDP/*.gcno StrategyDP/*.gcda StrategyDP/*.gcno
	rm -rf out

.PHONY: all test clean run-demo valgrind valgrind-demo valgrind-memento coverage coverage-demo coverage-memento coverage-clean