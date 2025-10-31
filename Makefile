# Compiler and flags
CXX = g++
CXXFLAGS = -g -std=c++11 -Wall -I. # <-- Add -I. here
LDFLAGS =

# Find all .cpp files in the current directory and subdirectories
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)

# Name of the final executable
TARGET = greenhouse
TEST_ORDER_TARGET = test_customer_order
CONCURRENT_DEMO_TARGET = concurrent_demo
MEMENTO_ADAPTER_TEST_TARGET = test_memento_adapter
INTEGRATED_TARGET = integrated_system
CUSTOMER_ORDER_TARGET = customer_order_test

.PHONY: all clean test valgrind integrated customer_order

all: $(TARGET)

# Integrated system target - runs both greenhouse simulation and customer order test
integrated: $(INTEGRATED_TARGET)

$(INTEGRATED_TARGET): $(filter-out main.o CustomerOrderTest.o, $(OBJS)) integrated_main.o
	$(CXX) $(LDFLAGS) -o $@ $^
	@echo "Integrated system built successfully!"
	@echo "Run with: ./$(INTEGRATED_TARGET)"

# Customer order test target - standalone order system test
customer_order: $(CUSTOMER_ORDER_TARGET)

$(CUSTOMER_ORDER_TARGET): $(filter-out main.o integrated_main.o, $(OBJS)) CustomerOrderTest.o
	$(CXX) $(LDFLAGS) -o $@ $^
	@echo "Customer order test built successfully!"
	@echo "Run with: ./$(CUSTOMER_ORDER_TARGET)"

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

# Generic rule to compile .cpp files to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) $(INTEGRATED_TARGET) $(CUSTOMER_ORDER_TARGET) *.gcda *.gcno *.gcov coverage.info
	rm -rf out

# Add your test and valgrind rules here if needed
test:
	@echo "No test target defined."

valgrind: all
	valgrind --leak-check=full ./$(TARGET)
