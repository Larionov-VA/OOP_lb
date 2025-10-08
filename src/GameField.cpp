#include "GameField.hpp"


GameField::GameField(std::unique_ptr<Entity> player, int width = 10, int height = 10, int level = 1) {
    if (width > MAX_FIELD_SIZE || height > MAX_FIELD_SIZE) {
        throw std::range_error("Max size of field is 25");
    }
    if (width < MIN_FIELD_SIZE || height < MIN_FIELD_SIZE) {
        throw std::range_error("Min size of field is 10");
    }
    this->widthField = width;
    this->heightField = height;
    this->gameLevel = level;
    generateFieldCells(std::move(player));
    generateEnemy();
}


void GameField::generateFieldCells(std::unique_ptr<Entity> player) {
    cells.reserve(widthField * heightField);
    for (int i = 0; i < widthField * heightField; ++i) {
        cells.emplace_back(i, i % widthField, i / widthField);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, widthField * heightField - 1);
    
    int randomPoint1, randomPoint2;
    const int minDistance = (widthField + heightField) / 3;
    
    do {
        randomPoint1 = dist(gen);
        randomPoint2 = dist(gen);
    } while (cells[randomPoint1].getDistance(cells[randomPoint2]) < minDistance);

    FieldCell& cell1 = cells[randomPoint1];
    FieldCell& cell2 = cells[randomPoint2];
    
    FieldCell& cell3 = [&]() -> FieldCell& {
        const int targetDist = (widthField + heightField) / 4;
        for (auto& cell : cells) {
            int dist1 = cell1.getDistance(cell);
            int dist2 = cell2.getDistance(cell);
            if (dist1 == dist2 && dist1 > targetDist) {
                cell.setAvaible(true);
                return cell;
            }
        }
        int midId = ((cell1.getCoord().second + cell2.getCoord().second) / 2) * widthField + 
                    ((cell1.getCoord().first + cell2.getCoord().first) / 2);
        return cells[midId];
    }();

    auto activateAround = [this](const FieldCell& center, int radius) {
        for (auto& cell : cells) {
            if (center.getDistance(cell) < radius) {
                cell.setAvaible(true);
            }
        }
    };

    auto activateLine = [this](const FieldCell& from, const FieldCell& to) {
        auto [x1, y1] = from.getCoord();
        auto [x2, y2] = to.getCoord();
        int dx = x2 - x1, dy = y2 - y1;
        int tolerance = abs(dx) + abs(dy) + widthField + heightField;
        
        for (auto& cell : cells) {
            auto [x, y] = cell.getCoord();
            if (abs((x - x1) * dy - (y - y1) * dx) <= tolerance) {
                cell.setAvaible(true);
            }
        }
    };

    activateAround(cell1, (widthField + heightField) / 6);
    activateAround(cell2, (widthField + heightField) / 6);
    activateAround(cell3, (widthField + heightField) / 4);
    activateLine(cell1, cell2);
    activateLine(cell1, cell3);
    
    spawnEntity(std::move(player), randomPoint1);
}


void GameField::moveEntity(int oldIndex, int newIndex) {
    entityManager.changeEntityIndex(oldIndex, newIndex);
    cells[oldIndex].setAvaible(true);
    cells[newIndex].setAvaible(false);
}


bool GameField::isMoveCorrect(int oldIndex, int newIndex) const {
    if (oldIndex < 0 || oldIndex >= widthField * heightField ||
        newIndex < 0 || newIndex >= widthField * heightField) {
        return false;
    }
    if (!cells[newIndex].isCellAvaible()) {
        return false;
    }
    int oldX = oldIndex % widthField;
    int newX = newIndex % widthField;
    if (abs(oldX - newX) > 1) {
        return false;
    }
    return true;
}


void GameField::spawnEntity(std::unique_ptr<Entity> entity, int index) {
    cells[index].setAvaible(false);
    entityManager.createEntity(std::move(entity), index);
}


void GameField::generateEnemy() {
    int countEnemy = sqrt((widthField + heightField) / 2) + gameLevel;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, widthField * heightField - 1);
    int playerIndex = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    do {
        int randomIndex = dist(gen);
        if (cells[randomIndex].isCellAvaible() && (cells[playerIndex].getDistance(cells[randomIndex]) > ((widthField + heightField) / 3))) {
            std::unique_ptr<Entity> enemy = std::make_unique<Enemy>();
            spawnEntity(std::move(enemy), randomIndex);
            --countEnemy;
        }
    } while (countEnemy >= 0);
}


void GameField::playerTurn() {
    char command;
    std::cin >> command;
    int playerIndex = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    int newPlayerIndex;
    switch (command) {
    case 'w':
        newPlayerIndex = playerIndex - widthField;
        if (isMoveCorrect(playerIndex, newPlayerIndex)) {
            moveEntity(playerIndex, newPlayerIndex);
        }
        break;
    case 'a':
        newPlayerIndex = playerIndex - 1;
        if (isMoveCorrect(playerIndex, newPlayerIndex)) {
            moveEntity(playerIndex, newPlayerIndex);
        }
        break;
    case 's':
        newPlayerIndex = playerIndex + widthField;
        if (isMoveCorrect(playerIndex, newPlayerIndex)) {
            moveEntity(playerIndex, newPlayerIndex);
        }
        break;
    case 'd':
        newPlayerIndex = playerIndex + 1;
        if (isMoveCorrect(playerIndex, newPlayerIndex)) {
            moveEntity(playerIndex, newPlayerIndex);
        }
        break;
    default:
        break;
    }
}


void GameField::summonsTurn() {

}


int GameField::getBestTurnForEnemy(int indexEnemy, int playerIndex, std::unordered_map<int, int>& visited) {
    visited[playerIndex] = 1;
    
    int up = playerIndex - widthField;
    int down = playerIndex + widthField;
    int left = playerIndex - 1;
    int right = playerIndex + 1;
    int result = -1;

    if (up == indexEnemy || down == indexEnemy || left == indexEnemy || right == indexEnemy) {
        return playerIndex;
    }
    if (isMoveCorrect(playerIndex, up) && !visited[up]) {
        result = getBestTurnForEnemy(indexEnemy, up, visited);
        if (result != -1) return result;
    }
    if (isMoveCorrect(playerIndex, down) && !visited[down]) {
        result = getBestTurnForEnemy(indexEnemy, down, visited);
        if (result != -1) return result;
    }
    if (isMoveCorrect(playerIndex, left) && !visited[left]) {
        result = getBestTurnForEnemy(indexEnemy, left, visited);
        if (result != -1) return result;
    }
    if (isMoveCorrect(playerIndex, right) && !visited[right]) {
        result = getBestTurnForEnemy(indexEnemy, right, visited);
        if (result != -1) return result;
    }
    return -1;
}


void GameField::enemyTurn() {
    int playerIndex = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    std::vector<int> enemyIndexes = entityManager.getIndexesWithEntity(Entity::entityType::ENEMY);
    for (int index : enemyIndexes) {
        std::unordered_map<int, int> visited{};

        int bestTurn = getBestTurnForEnemy(index, playerIndex, visited);
        if (isMoveCorrect(index, bestTurn)) {
            moveEntity(index, bestTurn);

        }
    }
}


void GameField::buildingsTurn() {

}


void GameField::show() {
    std::vector<int> enemyIndexes = entityManager.getIndexesWithEntity(Entity::entityType::ENEMY);
    int playerIndex = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    for (int i = 0; i < widthField * heightField; ++i) {
        Entity* currentEntity = entityManager[i];
        if (cells[i].isCellAvaible() || currentEntity) {
            
            if (entityManager[i]) {
                if (currentEntity->getType() == Entity::entityType::PLAYER) {
                    std::cout << "P";
                }
                else if (currentEntity->getType() == Entity::entityType::ENEMY) {
                    std::cout << "E";
                }
            }
            else {
                std::cout << "-";
            }
        }
        else {
            std::cout << "X";
            
        }
        if ((i + 1) % widthField == 0) {
            if (i == 0) {
                std::cout << "\tPlayer stats:";
            }
            if (i == 1) {
                std::cout << "\tintelligence " << 10 << "\t dexterity" << 10 << "strength " << 10;
            }
            std::cout << '\n';
        }
        else {
            std::cout << "  ";
        }
    }

}