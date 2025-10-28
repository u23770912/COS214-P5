CXX = g++
CXXFLAGS = -g -std=c++11 -Wall --coverage

# Source files
SRC = $(wildcard *.cpp) $(wildcard StateDP/*.cpp) $(wildcard BridgeDP/*.cpp) $(wildcard StrategyDP/*.cpp)
DEPS = $(wildcard *.h) $(wildcard StateDP/*.h) $(wildcard BridgeDP/*.h) $(wildcard StrategyDP/*.h)

# Object files - use proper path handling
OBJ = $(patsubst %.cpp,%.o,$(SRC))

# Exclude test files from main
MAIN_SRC = $(filter-out MoveToSaleFloorTest.cpp CustomerOrderTest.cpp,$(SRC))
MAIN_OBJ = $(patsubst %.cpp,%.o,$(MAIN_SRC))

# Test files - MoveToSalesFloor test (exclude main.cpp, Customer.cpp, InteractiveMode.cpp)
MOVE_TEST_SRC = $(filter-out main.cpp Customer.cpp InteractiveMode.cpp CustomerOrderTest.cpp,$(SRC))
MOVE_TEST_OBJ = $(patsubst %.cpp,%.o,$(MOVE_TEST_SRC))

# Customer Order test (exclude main.cpp and MoveToSaleFloorTest.cpp)
ORDER_TEST_SRC = $(filter-out main.cpp MoveToSaleFloorTest.cpp,$(SRC))
ORDER_TEST_OBJ = $(patsubst %.cpp,%.o,$(ORDER_TEST_SRC))

TARGET = greenhouse
TEST_MOVE_TARGET = test_move_to_sales_floor
TEST_ORDER_TARGET = test_customer_order

all: $(TARGET)

test: $(TEST_MOVE_TARGET) $(TEST_ORDER_TARGET)

$(TARGET): $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_MOVE_TARGET): $(MOVE_TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_ORDER_TARGET): $(ORDER_TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) $(TEST_MOVE_TARGET) $(TEST_ORDER_TARGET)
	rm -f *.gcda *.gcno *.gcov coverage.info StateDP/*.gcda StateDP/*.gcno
	rm -rf out

valgrind: $(TARGET)
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(TARGET)

coverage: all
	./$(TARGET)
	@echo "Generating coverage report..."
	@mkdir -p out
	@lcov --capture --directory . --output-file coverage.info
	@genhtml coverage.info --output-directory out
	@echo "Coverage report generated in out/index.html"

coverage-clean:
	rm -f *.gcda *.gcno *.gcov coverage.info StateDP/*.gcda StateDP/*.gcno
	rm -rf out

.PHONY: all test clean valgrind coverage-clean
