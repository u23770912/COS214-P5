# CXX = g++
# CXXFLAGS = -g -std=c++11 -Wall --coverage

# SRC = $(wildcard *.cpp)
# DEPS = $(wildcard *.h)
# OBJ = $(SRC:.cpp=.o)

# TARGET = greenhouse

# all: $(TARGET)

# $(TARGET): $(OBJ)
# 	$(CXX) $(CXXFLAGS) -o $@ $^

# %.o: %.cpp $(DEPS)
# 	$(CXX) $(CXXFLAGS) -c $<

# clean:
# 	rm -f $(OBJ) $(TARGET)
# 	rm -f *.gcda *.gcno *.gcov coverage.info
# 	rm -rf out

# valgrind: $(TARGET)
# 	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(TARGET)

# coverage: all
# 	./$(TARGET)
# 	@echo "Generating coverage report..."
# 	@mkdir -p out
# 	@lcov --capture --directory . --output-file coverage.info
# 	@genhtml coverage.info --output-directory out
# 	@echo "Coverage report generated in out/index.html"

# coverage-clean:
# 	rm -f *.gcda *.gcno *.gcov coverage.info
# 	rm -rf out

# .PHONY: all clean valgrind coverage-clean






# ==============================================
# This section builds the State & Strategy tests
# It includes compilation, linking, and coverage
# ==============================================

CXX = g++
CXXFLAGS = -g -std=c++11 -Wall --coverage

# Source files for State, Strategy, and Bridge patterns
SRC = StateStrategyTest.cpp \
      PlantProduct.cpp \
      InNurseryState.cpp PlantedState.cpp ReadyForSaleState.cpp WitheringState.cpp \
      CactusProfile.cpp FlowerProfile.cpp SucculentProfile.cpp TreeProfile.cpp \
      WateringStrategy.cpp GentleMistStrategy.cpp ArtisticPruningStrategy.cpp \
      FertilizingStrategy.cpp FloodWateringStrategy.cpp StandardPruningStrategy.cpp \
      DripWateringStrategy.cpp

# Object files derived from source files
OBJ = $(SRC:.cpp=.o)

TARGET = greenhouse_state_strategy

# Default target
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the compiled program
run: $(TARGET)
	@echo "🚀 Running program..."
	@./$(TARGET)

# Clean rule: removes build artifacts and any non-source files in all directories
clean:
	@echo "🧹 Cleaning project..."
	# Remove build products
	rm -f $(OBJ) $(TARGET)
	rm -f *.gcda *.gcno *.gcov coverage.info
	rm -rf out
	# Remove any non-.cpp or .h files from subdirectories
	find . -type f ! \( -name "*.cpp" -o -name "*.h" -o -name "Makefile" \) -delete
	@echo "✅ Clean complete."

# Run with Valgrind memory check
valgrind: $(TARGET)
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(TARGET)

# Generate coverage report
coverage: all
	./$(TARGET)
	@echo "📊 Generating coverage report..."
	@mkdir -p out
	@lcov --capture --directory . --output-file coverage.info
	@genhtml coverage.info --output-directory out
	@echo "✅ Coverage report generated in out/index.html"

# Clean only coverage-related files
coverage-clean:
	rm -f *.gcda *.gcno *.gcov coverage.info
	rm -rf out

.PHONY: all clean valgrind coverage coverage-clean run
