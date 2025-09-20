CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g
TARGET = game
SRCS = main.cpp Game.cpp GameField.cpp FieldCell.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp Game.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

Game.o: Game.cpp Game.hpp GameField.hpp
	$(CXX) $(CXXFLAGS) -c Game.cpp -o Game.o

GameField.o: GameField.cpp GameField.hpp FieldCell.hpp
	$(CXX) $(CXXFLAGS) -c GameField.cpp -o GameField.o

FieldCell.o: FieldCell.cpp FieldCell.hpp
	$(CXX) $(CXXFLAGS) -c FieldCell.cpp -o FieldCell.o

clean:
	rm -f $(TARGET) $(OBJS)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run