cmake_minimum_required(VERSION 3.16)
project(OOP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRCS
    src/core/CellState.cpp
    src/core/main.cpp
    src/core/Game.cpp
    src/core/GameField.cpp
    src/core/FieldCell.cpp
    src/core/EntityManager.cpp
    src/core/Config.cpp
    src/core/FileHandler.cpp
    src/core/InputController.cpp
    src/core/SavesManager.cpp
    src/core/visual/NCURSESVisualizer.cpp

    src/core/states/TrapEffect.cpp
    src/core/states/DecelerationEffect.cpp
    src/core/states/CoupreAnimate.cpp
    src/core/states/AttackEffect.cpp

    src/entities/Entity.cpp
    src/entities/Player.cpp
    src/entities/Enemy.cpp
    src/entities/EnemyBarracks.cpp
    src/entities/EnemyTower.cpp

    src/actor_utility/ActorAttributes.cpp
    src/actor_utility/ActorAttack.cpp
    src/actor_utility/ActorEquipment.cpp
    src/actor_utility/ActorHand.cpp
    src/actor_utility/ActorHealth.cpp
    src/actor_utility/ActorInventory.cpp
    src/actor_utility/ActorStats.cpp

    src/items/DirectDamageSpell.cpp
    src/items/AreaDamageSpell.cpp
    src/items/HealPotion.cpp
    src/items/UpgradeSpell.cpp
    src/items/TrapSpell.cpp
)

add_executable(game ${SRCS})

target_compile_options(game PRIVATE -Wall -Wextra -pthread)
target_include_directories(game PRIVATE src/)

target_link_libraries(game
    PRIVATE ncursesw
    PRIVATE bluetooth
)