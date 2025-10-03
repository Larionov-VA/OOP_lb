CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++14 -g
TARGET = game
SRCS = main.cpp Game.cpp GameField.cpp FieldCell.cpp Entity.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = Game.hpp GameField.hpp FieldCell.hpp Entity.hpp

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run