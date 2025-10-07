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

    infoMap[randomPoint1] = std::move(player);
    cells[randomPoint1].setAvaible(false);
}



void GameField::moveEntity(int oldIndex, int newIndex) {
    infoMap[newIndex] = std::move(infoMap[oldIndex]);
    infoMap.erase(oldIndex);
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


void GameField::spawnEnemy(int index) {
    cells[index].setAvaible(false);
    std::unique_ptr<Entity> enemy = std::make_unique<Enemy>();
    infoMap[index] = std::move(enemy);
}


void GameField::generateEnemy() {
    int countEnemy = sqrt((widthField + heightField) / 2) + gameLevel;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, widthField * heightField - 1);
    int playerIndex = getCellWithEntity(Entity::entityType::PLAYER);
    do {
        int randomIndex = dist(gen);
        if (cells[randomIndex].isCellAvaible() && (cells[playerIndex].getDistance(cells[randomIndex]) > ((widthField + heightField) / 3))) {
            spawnEnemy(randomIndex);
            --countEnemy;
        }
    } while (countEnemy >= 0);
}



void GameField::playerTurn() {
    char command;
    std::cin >> command;
    int index = getCellWithEntity(Entity::entityType::PLAYER);
    // std::cout << index << '\n';
    int newIndex;
    switch (command) {
    case 'w':
        newIndex = index - widthField;
        if (isMoveCorrect(index, newIndex)) {
            moveEntity(index, newIndex);
        }
        break;
    case 'a':
        newIndex = index - 1;
        if (isMoveCorrect(index, newIndex)) {
            moveEntity(index, newIndex);
        }
        break;
    case 's':
        newIndex = index + widthField;
        if (isMoveCorrect(index, newIndex)) {
            moveEntity(index, newIndex);
        }
        break;
    case 'd':
        newIndex = index + 1;
        if (isMoveCorrect(index, newIndex)) {
            moveEntity(index, newIndex);
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
    
    if (up == indexEnemy || down == indexEnemy || left == indexEnemy || right == indexEnemy) {
        return playerIndex;
    }
    
    int result = -1;

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
    int playerIndex = getCellWithEntity(Entity::entityType::PLAYER);
    
    int enemyfirstIndex = getCellWithEntity(Entity::entityType::ENEMY);
    std::unordered_map<int, int> visited{};
    int bestTurn = getBestTurnForEnemy(enemyfirstIndex, playerIndex, visited);
    if (isMoveCorrect(enemyfirstIndex, bestTurn)) {
        moveEntity(enemyfirstIndex, bestTurn);
    }
}

void GameField::buildingsTurn() {

}


int GameField::getCellWithEntity(Entity::entityType type) {
    if (infoMap.empty()) {
        return -1;
    }
    for (const auto& pair : infoMap) {
        if (pair.second && pair.second->getType() == type) {
            return pair.first;
        }
    }
    return -1;
}


void GameField::show() {
    for (int i = 0; i < widthField * heightField; ++i) {
        if (cells[i].isCellAvaible() || infoMap[i]) {
            // std::cout << (int)infoMap[i]->getType();
            if (infoMap[i]) {
                if (infoMap[i]->getType() == Entity::entityType::PLAYER) {
                    std::cout << "P";
                }
                else if (infoMap[i]->getType() == Entity::entityType::ENEMY) {
                    std::cout << "E";
                }
            }
            else {
                std::cout << "-";
            }
            // std::cout << "-";
        }
        else {
            std::cout << "X";
            
        }
        // std::cout << std::setw(3) << cells[i].getID();
        if ((i + 1) % widthField == 0) {
            std::cout << '\n';
        }
        else {
            std::cout << "  ";
        }
    }
}