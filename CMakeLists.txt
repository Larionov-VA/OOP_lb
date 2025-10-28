cmake_minimum_required(VERSION 3.16)
project(OOP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(external/ftxui)


set(SRCS
    src/core/main.cpp
    src/core/Game.cpp
    src/core/GameField.cpp
    src/core/FieldCell.cpp
    src/core/EntityManager.cpp
    src/core/Visualizer.cpp    
    src/core/Config.cpp

    src/entities/Entity.cpp
    src/entities/Player.cpp
    src/entities/Enemy.cpp
    src/entities/EnemyBarracks.cpp

    src/actor_utility/ActorAtributes.cpp
    src/actor_utility/ActorAttack.cpp
    src/actor_utility/ActorEquipment.cpp
    src/actor_utility/ActorHand.cpp
    src/actor_utility/ActorHealth.cpp
    src/actor_utility/ActorInventory.cpp
    src/actor_utility/ActorStats.cpp
)


add_executable(game ${SRCS})


target_compile_options(game PRIVATE -Wall -Wextra -g)
target_include_directories(game PRIVATE src/)


target_link_libraries(game
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
)