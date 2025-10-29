CXX = g++
CXXFLAGS = -g -std=c++11 -Wall #--coverage

SRC = $(filter-out CactusProfile.cpp AutonomousMode.cpp InteractiveMode.cpp, $(wildcard *.cpp))
DEPS = $(wildcard *.h)
OBJ = $(SRC:.cpp=.o)

TARGET = greenhouse

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)
	rm -f *.gcda *.gcno *.gcov coverage.info
	rm -rf out

valgrind: $(TARGET)
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(TARGET)

coverage: all
	./$(TARGET)
	@echo "Generating coverage report..."
	@mkdir -p out
	@lcov --capture --directory . --output-file coverage.info
	@genhtml coverage.info --output-directory out
	@echo "Coverage report generated in out/index.html"

coverage-clean:
	rm -f *.gcda *.gcno *.gcov coverage.info
	rm -rf out

.PHONY: all clean valgrind coverage-clean
