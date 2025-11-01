# Compiler and flags
CXX = g++
CXXFLAGS = -g -std=c++11 -Wall -I. # <-- Add -I. here
LDFLAGS =

# Find all .cpp files in the current directory and subdirectories
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)

# Name of the final executable
TARGET = greenhouse

.PHONY: all clean test valgrind

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

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
