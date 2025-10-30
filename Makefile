CXX = g++
CXXFLAGS = -g -std=c++11 -Wall --coverage

# Source files
SRC = $(wildcard *.cpp) $(wildcard StateDP/*.cpp) $(wildcard BridgeDP/*.cpp) $(wildcard StrategyDP/*.cpp)
DEPS = $(wildcard *.h) $(wildcard StateDP/*.h) $(wildcard BridgeDP/*.h) $(wildcard StrategyDP/*.h)

# Object files - use proper path handling
OBJ = $(patsubst %.cpp,%.o,$(SRC))

# Exclude test files from main
MAIN_SRC = $(filter-out CustomerOrderTest.cpp,$(SRC))
MAIN_OBJ = $(patsubst %.cpp,%.o,$(MAIN_SRC))

# Customer Order test (exclude main.cpp)
ORDER_TEST_SRC = $(filter-out main.cpp ConcurrentOrderDemo.cpp,$(SRC))
ORDER_TEST_OBJ = $(patsubst %.cpp,%.o,$(ORDER_TEST_SRC))

# Concurrent Demo (exclude main.cpp and CustomerOrderTest.cpp)
CONCURRENT_DEMO_SRC = $(filter-out main.cpp CustomerOrderTest.cpp,$(SRC))
CONCURRENT_DEMO_OBJ = $(patsubst %.cpp,%.o,$(CONCURRENT_DEMO_SRC))

TARGET = greenhouse
TEST_ORDER_TARGET = test_customer_order
CONCURRENT_DEMO_TARGET = concurrent_demo

all: $(TARGET)

test: $(TEST_ORDER_TARGET)

concurrent: $(CONCURRENT_DEMO_TARGET)

$(TARGET): $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_ORDER_TARGET): $(ORDER_TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(CONCURRENT_DEMO_TARGET): $(CONCURRENT_DEMO_OBJ)
	$(CXX) $(CXXFLAGS) -lpthread -o $@ $^

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) $(TEST_ORDER_TARGET) $(CONCURRENT_DEMO_TARGET)
	rm -f *.gcda *.gcno *.gcov coverage.info StateDP/*.gcda StateDP/*.gcno
	rm -rf out

valgrind: $(TARGET)
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(TARGET)

valgrind-test: $(TEST_ORDER_TARGET)
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all --verbose ./$(TEST_ORDER_TARGET)

valgrind-concurrent: $(CONCURRENT_DEMO_TARGET)
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all --verbose ./$(CONCURRENT_DEMO_TARGET)

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

.PHONY: all test concurrent clean valgrind valgrind-test valgrind-concurrent coverage-clean
