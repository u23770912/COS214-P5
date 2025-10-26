# Compiler
CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

# Source files
SRCS = main.cpp Order.cpp OrderMemento.cpp OrderHistory.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable
TARGET = order_app

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile .cpp to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJS) $(TARGET)
