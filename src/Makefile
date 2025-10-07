CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g
TARGET = game
SRCS = main.cpp Game.cpp GameField.cpp FieldCell.cpp Entity.cpp Player.cpp Enemy.cpp ActorHealth.cpp ActorEquipment.cpp ActorAttack.cpp ActorAtributes.cpp EntityManager.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = Game.hpp GameField.hpp FieldCell.hpp Entity.hpp Player.hpp Enemy.hpp ActorHealth.hpp ActorEquipment.hpp ActorAttack.hpp ActorAtributes.hpp EntityManager.hpp

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