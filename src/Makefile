CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g
TARGET = game
SRCS = main.cpp Game.cpp GameField.cpp FieldCell.cpp Entity.cpp Player.cpp Enemy.cpp ActorHealth.cpp ActorEquipment.cpp ActorAttack.cpp ActorAtributes.cpp EntityManager.cpp
OBJ_DIR = obj
OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
DEPS = Game.hpp GameField.hpp FieldCell.hpp Entity.hpp Player.hpp Enemy.hpp ActorHealth.hpp ActorEquipment.hpp ActorAttack.hpp ActorAtributes.hpp EntityManager.hpp

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.cpp $(DEPS) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(TARGET) $(OBJ_DIR)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run