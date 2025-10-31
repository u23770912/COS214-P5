# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic
DEBUG_FLAGS = -g
RELEASE_FLAGS = -O2

# Target executable
TARGET = pots

# Source files
SRCS = main.cpp \
       Pot.cpp \
       PotFactory.cpp \
       ClayPot.cpp \
       ClayPotFactory.cpp \
       PlasticPot.cpp \
       PlasticPotFactory.cpp \
       WoodenPot.cpp \
       WoodenPotFactory.cpp \
       MetalPot.cpp \
       MetalPotFactory.cpp \
       GlassPot.cpp \
       GlassPotFactory.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Header files
HEADERS = Pot.h PotFactory.h \
          ClayPot.h ClayPotFactory.h \
          PlasticPot.h PlasticPotFactory.h \
          WoodenPot.h WoodenPotFactory.h \
          MetalPot.h MetalPotFactory.h \
          GlassPot.h GlassPotFactory.h

# Default target
all: $(TARGET)

# Create executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile source files to object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Debug build
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

# Release build
release: CXXFLAGS += $(RELEASE_FLAGS)
release: $(TARGET)

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

# Clean everything including executable
distclean: clean
	rm -f $(TARGET)

# Phony targets
.PHONY: all debug release clean distclean