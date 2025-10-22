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
        cells.emplace_back(i, i % widthField, i / heightField, false);
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

    for (int i = widthField; i < widthField * heightField - widthField; ++i) {
        int up = i - widthField;
        int down = i + widthField;
        int left = i - 1;
        int right = i + 1;
        if (cells[i].isCellAvaible() && (!cells[up].isCellAvaible() || !cells[down].isCellAvaible() ||
            !cells[left].isCellAvaible() || !cells[right].isCellAvaible()) && (i % widthField)) {
            cells[i].setSlow(true);
        }
    }

    spawnEntity(std::make_unique<EnemyBarracks>(), randomPoint2);
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


int GameField::firstEnemyIndexOnLine(int oldIndex, int newIndex) const {
    int totalCells = widthField * heightField;
    int rowOld = oldIndex / widthField;
    int rowNew = newIndex / widthField;


    bool isHorizontal = (rowOld == rowNew);

    if (isHorizontal) {
        int dir = (newIndex > oldIndex) ? 1 : -1;
        int current = oldIndex + dir;

        while (current >= 0 && current < totalCells && (current / widthField) == rowOld) {
            if (entityManager[current] &&
                (entityManager[current]->getType() == Entity::entityType::ENEMY ||
                entityManager[current]->getType() == Entity::entityType::BARRACKS))
                return current;
            current += dir;
        }
    } else {
        int dir = (newIndex > oldIndex) ? widthField : -widthField;
        int current = oldIndex + dir;

        while (current >= 0 && current < totalCells) {
            if (entityManager[current] &&
                (entityManager[current]->getType() == Entity::entityType::ENEMY ||
                entityManager[current]->getType() == Entity::entityType::BARRACKS))
                return current;
            current += dir;
        }
    }
    return -1;
}


void GameField::playerTurn() {
    char command;
    std::cin >> command;
    int playerIndex = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    int newPlayerIndex;
    int enemyIndex;
    switch (command) {
    case 'w':
        if (entityManager[playerIndex]->checkDebaffState()) {
            break;
        }
        newPlayerIndex = playerIndex - widthField;
        enemyIndex = firstEnemyIndexOnLine(playerIndex, newPlayerIndex);
        if (entityManager[newPlayerIndex]) {
            entityManager[newPlayerIndex]->causeDamage(entityManager[playerIndex]->getDamage());
        }
        else if (enemyIndex != -1 && !entityManager[playerIndex]->melle()) {
            entityManager[enemyIndex]->causeDamage(entityManager[playerIndex]->getDamage());
        }
        else if (isMoveCorrect(playerIndex, newPlayerIndex)) {
            if (cells[newPlayerIndex].isCellSlow()) {
                entityManager[playerIndex]->setDebaffState();
            }
            moveEntity(playerIndex, newPlayerIndex);
        }
        break;
    case 'a':
        if (entityManager[playerIndex]->checkDebaffState()) {
            break;
        }
        newPlayerIndex = playerIndex - 1;
        enemyIndex = firstEnemyIndexOnLine(playerIndex, newPlayerIndex);
        if (entityManager[newPlayerIndex]) {
            entityManager[newPlayerIndex]->causeDamage(entityManager[playerIndex]->getDamage());
        }
        else if (enemyIndex != -1 && !entityManager[playerIndex]->melle()) {
            entityManager[enemyIndex]->causeDamage(entityManager[playerIndex]->getDamage());
        }
        else if (isMoveCorrect(playerIndex, newPlayerIndex)) {
            if (cells[newPlayerIndex].isCellSlow()) {
                entityManager[playerIndex]->setDebaffState();
            }
            moveEntity(playerIndex, newPlayerIndex);
        }
        break;
    case 's':
        if (entityManager[playerIndex]->checkDebaffState()) {
            break;
        }
        newPlayerIndex = playerIndex + widthField;
        enemyIndex = firstEnemyIndexOnLine(playerIndex, newPlayerIndex);
        if (entityManager[newPlayerIndex]) {
            entityManager[newPlayerIndex]->causeDamage(entityManager[playerIndex]->getDamage());
        }
        else if (enemyIndex != -1 && !entityManager[playerIndex]->melle()) {
            entityManager[enemyIndex]->causeDamage(entityManager[playerIndex]->getDamage());
        }
        else if (isMoveCorrect(playerIndex, newPlayerIndex)) {
            if (cells[newPlayerIndex].isCellSlow()) {
                entityManager[playerIndex]->setDebaffState();
            }
            moveEntity(playerIndex, newPlayerIndex);
        }
        break;
    case 'd':
        if (entityManager[playerIndex]->checkDebaffState()) {
            break;
        }
        newPlayerIndex = playerIndex + 1;
        enemyIndex = firstEnemyIndexOnLine(playerIndex, newPlayerIndex);
        if (entityManager[newPlayerIndex]) {
            entityManager[newPlayerIndex]->causeDamage(entityManager[playerIndex]->getDamage());
        }
        else if (enemyIndex != -1 && !entityManager[playerIndex]->melle()) {
            entityManager[enemyIndex]->causeDamage(entityManager[playerIndex]->getDamage());
        }
        else if (isMoveCorrect(playerIndex, newPlayerIndex)) {
            if (cells[newPlayerIndex].isCellSlow()) {
                entityManager[playerIndex]->setDebaffState();
            }
            moveEntity(playerIndex, newPlayerIndex);
        }
        break;
    case 'q':
        entityManager[playerIndex]->swapWeapon();
    default:
        break;
    }
}


void GameField::summonsTurn() {

}

void GameField::update() {
    std::vector<int> enemyIndexes = entityManager.getIndexesWithEntity(Entity::entityType::ENEMY);
    for (int index : enemyIndexes) {
        if (!entityManager[index]->alive()) {
            cells[index].setAvaible(true);
            entityManager.killEntity(index);
        }
    }
    std::vector<int> barrackIndexes = entityManager.getIndexesWithEntity(Entity::entityType::BARRACKS);
    if (!barrackIndexes.empty()) {
        for (int index : barrackIndexes) {
            if (!entityManager[index]->alive()) {
                cells[index].setAvaible(true);
                entityManager.killEntity(index);
            }
        }
    }
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
        else if (cells[playerIndex].getDistance(cells[index]) <= 1) {
            entityManager[playerIndex]->causeDamage(entityManager[index]->getDamage());
        }
    }
}


void GameField::buildingsTurn() {
    std::vector<int> barrackIndexes = entityManager.getIndexesWithEntity(Entity::entityType::BARRACKS);
    int index;
    if (barrackIndexes.empty()) {
        return;
    }
    else {
        index = barrackIndexes[0];
    }
    if (entityManager[index]->timeToSpawn()) {
        int up = index - widthField;
        int down = index + widthField;
        int left = index - 1;
        int right = index + 1;

        if (isMoveCorrect(index, up)) {
            spawnEntity(std::make_unique<Enemy>(), up);
        }
        else if (isMoveCorrect(index, down)) {
            spawnEntity(std::make_unique<Enemy>(), down);
        }
        else if (isMoveCorrect(index, left)) {
            spawnEntity(std::make_unique<Enemy>(), left);
        }
        else if (isMoveCorrect(index, right)) {
            spawnEntity(std::make_unique<Enemy>(), right);
        }
    }
}


bool GameField::playerAlive() const {
    int playerIndex = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    return entityManager[playerIndex]->alive();
}


void GameField::show() {
    std::vector<int> enemyIndexes = entityManager.getIndexesWithEntity(Entity::entityType::ENEMY);
    int playerIndex = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    for (int i = 0; i < widthField * heightField; ++i) {
        Entity* currentEntity = entityManager[i];
        if (cells[i].isCellAvaible() || currentEntity) {
            
            if (entityManager[i]) {
                if (currentEntity->getType() == Entity::entityType::PLAYER) {
                    // std::cout << "P";
                    fieldChars.push_back('P');
                }
                else if (currentEntity->getType() == Entity::entityType::ENEMY) {
                    // std::cout << "E";
                    fieldChars.push_back('E');
                }
                else if (currentEntity->getType() == Entity::entityType::BARRACKS) {
                    // std::cout << "B";
                    fieldChars.push_back('B');
                }
            }
            else if (cells[i].isCellSlow()) {
                // std::cout << "=";
                fieldChars.push_back('=');
            }
            else {
                // std::cout << "-";
                fieldChars.push_back('-');
            }
        }
        else {
            // std::cout << "X";
            fieldChars.push_back('X');
        }
        // if ((i + 1) % widthField == 0) {
        //     if (i / widthField == 0) {
        //         std::cout << "\tPlayer stats:";
        //     }
        //     if (i / widthField == 1) {
        //         std::cout << "\tint " << entityManager[playerIndex]->getInt() << "\tdex " << entityManager[playerIndex]->getDex() << "\tstr " << entityManager[playerIndex]->getStr();
        //     }
        //     if (i / widthField == 3) {
        //         std::cout << "\tLife\t" << entityManager[playerIndex]->getHealth().first << "|"  << entityManager[playerIndex]->getHealth().second;
        //     }
        //     if (i / widthField == 4) {
        //         std::cout << "\tAttack\t" << entityManager[playerIndex]->getDamage();
        //     }
        //     if (i / widthField == 6) {
        //         std::cout << "\tCurrent weapon:\t" << (entityManager[playerIndex]->melle() ? "Sword" : "Bow");
        //     }
        //     if (i / widthField == 8) {
        //         std::cout << "\tEnemy count:\t" << enemyIndexes.size();
        //     }
        //     std::cout << '\n';
        // }
        // else {
        //     std::cout << "  ";
        // }
    }
}