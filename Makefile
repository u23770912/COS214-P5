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

# Customer Order test (exclude main.cpp and other test files)
ORDER_TEST_SRC = $(filter-out main.cpp MoveToSaleFloorTest.cpp PureObserverPatternTest.cpp DirectorPatternDemo.cpp FacadePatternDemo.cpp,$(SRC))
ORDER_TEST_OBJ = $(patsubst %.cpp,%.o,$(ORDER_TEST_SRC))

# Pure Observer Pattern test (exclude main.cpp, MoveToSaleFloorTest.cpp, CustomerOrderTest.cpp)
PURE_OBSERVER_TEST_SRC = $(filter-out main.cpp MoveToSaleFloorTest.cpp CustomerOrderTest.cpp DirectorPatternDemo.cpp,$(SRC))
PURE_OBSERVER_TEST_OBJ = $(patsubst %.cpp,%.o,$(PURE_OBSERVER_TEST_SRC))

# Director Pattern demo (exclude main.cpp and test files)
DIRECTOR_DEMO_SRC = $(filter-out main.cpp MoveToSaleFloorTest.cpp CustomerOrderTest.cpp PureObserverPatternTest.cpp FacadePatternDemo.cpp,$(SRC))
DIRECTOR_DEMO_OBJ = $(patsubst %.cpp,%.o,$(DIRECTOR_DEMO_SRC))

# Facade Pattern demo (exclude main.cpp and test files)
FACADE_DEMO_SRC = $(filter-out main.cpp MoveToSaleFloorTest.cpp CustomerOrderTest.cpp PureObserverPatternTest.cpp DirectorPatternDemo.cpp,$(SRC))
FACADE_DEMO_OBJ = $(patsubst %.cpp,%.o,$(FACADE_DEMO_SRC))

TARGET = greenhouse
TEST_MOVE_TARGET = test_move_to_sales_floor
TEST_ORDER_TARGET = test_customer_order
TEST_PURE_OBSERVER_TARGET = test_pure_observer
DIRECTOR_DEMO_TARGET = director_demo
FACADE_DEMO_TARGET = facade_demo

all: $(TARGET)

test: $(TEST_MOVE_TARGET) $(TEST_ORDER_TARGET) $(TEST_PURE_OBSERVER_TARGET)

demo: $(DIRECTOR_DEMO_TARGET) $(FACADE_DEMO_TARGET)

$(TARGET): $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_MOVE_TARGET): $(MOVE_TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_ORDER_TARGET): $(ORDER_TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_PURE_OBSERVER_TARGET): $(PURE_OBSERVER_TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(DIRECTOR_DEMO_TARGET): $(DIRECTOR_DEMO_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(FACADE_DEMO_TARGET): $(FACADE_DEMO_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) $(TEST_MOVE_TARGET) $(TEST_ORDER_TARGET) $(TEST_PURE_OBSERVER_TARGET) $(DIRECTOR_DEMO_TARGET) $(FACADE_DEMO_TARGET)
	rm -f *.gcda *.gcno *.gcov coverage.info StateDP/*.gcda StateDP/*.gcno
	rm -rf out

valgrind: $(TARGET)
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(TARGET)

valgrind-customer: $(TEST_ORDER_TARGET)
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all --verbose ./$(TEST_ORDER_TARGET)

valgrind-move: $(TEST_MOVE_TARGET)
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(TEST_MOVE_TARGET)

valgrind-observer: $(TEST_PURE_OBSERVER_TARGET)
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(TEST_PURE_OBSERVER_TARGET)

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

.PHONY: all test clean valgrind valgrind-customer valgrind-move valgrind-observer coverage-clean
