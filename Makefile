# Compiler and flags
CXX = g++
CXXFLAGS = -g -std=c++11 -Wall -I.
LDFLAGS =

# Find all .cpp files in current directory and subdirectories
SRCS := $(wildcard *.cpp) $(wildcard SuggestionTemplate/*.cpp)
OBJS := $(SRCS:.cpp=.o)

# Executable targets
INTEGRATED_TARGET = integrated_system
DEMO_TARGET = demo

.PHONY: all clean integrated demo

# Build both executables by default
all: integrated demo

# Integrated system target - complete greenhouse simulation + customer interaction
integrated: $(INTEGRATED_TARGET)

$(INTEGRATED_TARGET): $(filter-out main.o CustomerOrderTest.o DemoMain.o newMain.o, $(OBJS)) integrated_main.o
	$(CXX) $(LDFLAGS) -o $@ $^
	@echo "✓ Integrated system built successfully!"
	@echo "  Run with: ./$(INTEGRATED_TARGET)"

# Demo target - demonstration of design patterns
demo: $(DEMO_TARGET)

$(DEMO_TARGET): $(filter-out main.o CustomerOrderTest.o integrated_main.o newMain.o, $(OBJS)) DemoMain.o
	$(CXX) $(LDFLAGS) -o $@ $^
	@echo "✓ Demo built successfully!"
	@echo "  Run with: ./$(DEMO_TARGET)"

# Generic rule to compile .cpp files to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean all build artifacts
clean:
	rm -f $(OBJS) $(INTEGRATED_TARGET) $(DEMO_TARGET)
	rm -f SuggestionTemplate/*.o
	rm -f *.gcda *.gcno *.gcov coverage.info
	rm -rf out
	@echo "✓ Clean complete"

# Convenience targets
run-integrated: integrated
	./$(INTEGRATED_TARGET)

run-demo: demo
	./$(DEMO_TARGET)

# Memory leak checking
valgrind-integrated: integrated
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(INTEGRATED_TARGET)

valgrind-demo: demo
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(DEMO_TARGET)
