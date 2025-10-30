CXX = g++
CXXFLAGS = -g -std=c++11 -Wall --coverage

# Source files
SRC = $(wildcard *.cpp) $(wildcard StateDP/*.cpp) $(wildcard BridgeDP/*.cpp) $(wildcard StrategyDP/*.cpp)
DEPS = $(wildcard *.h) $(wildcard StateDP/*.h) $(wildcard BridgeDP/*.h) $(wildcard StrategyDP/*.h)

# Object files - use proper path handling
OBJ = $(patsubst %.cpp,%.o,$(SRC))

# Exclude test files from main
MAIN_SRC = $(filter-out CustomerOrderTest.cpp test_memento_adapter.cpp newMain.cpp,$(SRC))
MAIN_OBJ = $(patsubst %.cpp,%.o,$(MAIN_SRC))

# Customer Order test (exclude main.cpp and concurrent/memento tests)  
ORDER_TEST_SRC = $(filter-out main.cpp ConcurrentOrderDemo.cpp test_memento_adapter.cpp newMain.cpp,$(SRC))
ORDER_TEST_OBJ = $(patsubst %.cpp,%.o,$(ORDER_TEST_SRC))

# Concurrent Demo (exclude main.cpp, CustomerOrderTest.cpp, and memento tests)
CONCURRENT_DEMO_SRC = $(filter-out main.cpp CustomerOrderTest.cpp test_memento_adapter.cpp newMain.cpp,$(SRC))
CONCURRENT_DEMO_OBJ = $(patsubst %.cpp,%.o,$(CONCURRENT_DEMO_SRC))

# Memento & Adapter test (exclude main.cpp and other test files)
MEMENTO_ADAPTER_TEST_SRC = $(filter-out main.cpp CustomerOrderTest.cpp ConcurrentOrderDemo.cpp,$(SRC))
MEMENTO_ADAPTER_TEST_OBJ = $(patsubst %.cpp,%.o,$(MEMENTO_ADAPTER_TEST_SRC))

TARGET = greenhouse
TEST_ORDER_TARGET = test_customer_order
CONCURRENT_DEMO_TARGET = concurrent_demo
MEMENTO_ADAPTER_TEST_TARGET = test_memento_adapter

all: $(TARGET)

test: $(TEST_ORDER_TARGET) $(MEMENTO_ADAPTER_TEST_TARGET)

concurrent: $(CONCURRENT_DEMO_TARGET)

$(TARGET): $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_ORDER_TARGET): $(ORDER_TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(CONCURRENT_DEMO_TARGET): $(CONCURRENT_DEMO_OBJ)
	$(CXX) $(CXXFLAGS) -lpthread -o $@ $^

$(MEMENTO_ADAPTER_TEST_TARGET): $(MEMENTO_ADAPTER_TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) $(TEST_ORDER_TARGET) $(CONCURRENT_DEMO_TARGET) $(MEMENTO_ADAPTER_TEST_TARGET)
	rm -f *.gcda *.gcno *.gcov coverage.info StateDP/*.gcda StateDP/*.gcno BridgeDP/*.gcda BridgeDP/*.gcno StrategyDP/*.gcda StrategyDP/*.gcno
	rm -rf out

valgrind: $(TARGET)
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(TARGET)

valgrind-test: $(TEST_ORDER_TARGET)
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all --verbose ./$(TEST_ORDER_TARGET)

valgrind-concurrent: $(CONCURRENT_DEMO_TARGET)
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all --verbose ./$(CONCURRENT_DEMO_TARGET)

valgrind-memento: $(MEMENTO_ADAPTER_TEST_TARGET)
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(MEMENTO_ADAPTER_TEST_TARGET)

coverage: all
	./$(TARGET)
	@echo "Generating coverage report..."
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

.PHONY: all test concurrent clean valgrind valgrind-test valgrind-concurrent valgrind-memento coverage coverage-memento coverage-clean
