# Compiler and flags
CXX = g++
CXXFLAGS = -g -std=c++11 -Wall -I. -IPotDecorator `pkg-config --cflags gtk+-3.0`
LDFLAGS = -pthread `pkg-config --libs gtk+-3.0`

# Find all .cpp files in the current directory and subdirectories
SRCS := $(wildcard *.cpp) $(wildcard PotDecorator/*.cpp)
# Exclude test files with their own main() functions
SRCS := $(filter-out CustomerOrderTest.cpp DemoMain.cpp main_option_a_backup.cpp builder_Testing_main.cpp integrated_main.cpp, $(SRCS))
OBJS := $(SRCS:.cpp=.o)

# Name of the final executable
TARGET = greenhouse

.PHONY: all clean test valgrind

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Generic rule to compile .cpp files to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) *.gcda *.gcno *.gcov coverage.info
	rm -rf out

# Add your test and valgrind rules here if needed
test:
	@echo "No test target defined."

valgrind: all
	valgrind --leak-check=full ./$(TARGET)