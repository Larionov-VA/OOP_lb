cmake_minimum_required(VERSION 3.16)
project(OOP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# add_subdirectory(external/ftxui)


set(SRCS
    src/main.cpp
    src/Game.cpp
    src/GameField.cpp
    src/FieldCell.cpp
    src/Entity.cpp
    src/Player.cpp
    src/Enemy.cpp
    src/ActorHealth.cpp
    src/ActorEquipment.cpp
    src/ActorAttack.cpp
    src/ActorAtributes.cpp
    src/EntityManager.cpp
    # src/Visualizer.cpp
    src/EnemyBarracks.cpp
)


add_executable(game ${SRCS})


target_compile_options(game PRIVATE -Wall -Wextra -g)
target_include_directories(game PRIVATE src/)


# target_link_libraries(game
#     PRIVATE ftxui::screen
#     PRIVATE ftxui::dom
#     PRIVATE ftxui::component
# )