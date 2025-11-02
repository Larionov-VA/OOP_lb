#include "GameField.hpp"
#include "GameContext.hpp"

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
        cells.emplace_back(i, i % widthField, i / widthField, false);
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
    spawnEntity(std::make_unique<EnemyTower>(gameLevel), cell3.getIndex());
    spawnEntity(std::make_unique<EnemyBarracks>(gameLevel), randomPoint2);
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
    int countEnemy = sqrt(((widthField + heightField) / 2) + gameLevel);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, widthField * heightField - 1);
    int playerIndex = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    do {
        int randomIndex = dist(gen);
        bool randomCellAvailable = cells[randomIndex].isCellAvaible();
        float distanceToPlayer = cells[playerIndex].getDistance(cells[randomIndex]);
        if (randomCellAvailable && (distanceToPlayer > ((widthField + heightField) / 3))) {
            std::unique_ptr<Entity> enemy = std::make_unique<Enemy>(gameLevel);
            spawnEntity(std::move(enemy), randomIndex);
            --countEnemy;
        }
    } while (countEnemy >= 0);
}


bool GameField::enemyOnIndex(int index) const {
    if (entityManager[index]) {
        auto type = entityManager[index]->getType();
        if (type == Entity::entityType::ENEMY ||
            type == Entity::entityType::BARRACKS ||
            type == Entity::entityType::TOWER) {
                return true;
        }
    }
    return false;
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
            if (enemyOnIndex(current)) {
                return current;
            }
            current += dir;
        }
    } else {
        int dir = (newIndex > oldIndex) ? widthField : -widthField;
        int current = oldIndex + dir;
        while (current >= 0 && current < totalCells) {
            if (enemyOnIndex(current)) {
                return current;
            }
            current += dir;
        }
    }
    return -1;
}


bool GameField::playerTurn(char command) {
    int playerIndex = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    GameContext ctx{cells, entityManager};
    int newPlayerIndex;
    int enemyIndex;
    int powerOfSpells;
    bool move = false;
    bool cellSlowed = entityManager[playerIndex]->checkDebaffState();
    switch (command) {
    case 'w':
        if (cellSlowed) {
            break;
        }
        newPlayerIndex = playerIndex - widthField;
        move = true;
        break;
    case 'a':
        if (cellSlowed) {
            break;
        }
        newPlayerIndex = playerIndex - 1;
        move = true;
        break;
    case 's':
        if (cellSlowed) {
            break;
        }
        newPlayerIndex = playerIndex + widthField;
        move = true;
        break;
    case 'd':
        if (cellSlowed) {
            break;
        }
        newPlayerIndex = playerIndex + 1;
        move = true;
        break;
    case 'e':
        powerOfSpells = entityManager[playerIndex]->getPowerOfSpell();
        entityManager[playerIndex]->useItem(ctx, playerIndex, powerOfSpells);
        return true;
        break;
    case '0':
    case '1':
    case '2':
    case '3':
        entityManager[playerIndex]->swapItemInHand(command-'0');
        break;
    case 'q':
        entityManager[playerIndex]->swapWeapon();
        return true;
    default:
        break;
    }
    if (move) {
        enemyIndex = firstEnemyIndexOnLine(playerIndex, newPlayerIndex);
        int playerDamage = entityManager[playerIndex]->getDamage();
        if (entityManager[newPlayerIndex]) {
            /*newPlayerIndex == enemyIndex если entityManager[newPlayerIndex] занят*/
            entityManager[newPlayerIndex]->causeDamage(playerDamage);
        }
        else if (enemyIndex != -1 && !entityManager[playerIndex]->melle()) {
            /*дальний бой*/
            entityManager[enemyIndex]->causeDamage(playerDamage);
        }
        else if (isMoveCorrect(playerIndex, newPlayerIndex)) {
            /*наложение дебафа*/
            if (cells[newPlayerIndex].isCellSlow()) {
                entityManager[playerIndex]->setDebaffState();
            }
            /*передвижение*/
            moveEntity(playerIndex, newPlayerIndex);
        }
    }
    return move;
}


int GameField::getCountOfEnemy() {
    std::vector<int> enemyIndexes = entityManager.getIndexesWithEntity(Entity::entityType::ENEMY);
    std::vector<int> barrackIndexes = entityManager.getIndexesWithEntity(Entity::entityType::BARRACKS);
    std::vector<int> towerIndexes = entityManager.getIndexesWithEntity(Entity::entityType::TOWER);
    return enemyIndexes.size() + barrackIndexes.size() + towerIndexes.size();
}


void GameField::playerLevelUp(char attribute) {
    int playerIndex = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    int prevInt = entityManager[playerIndex]->getInt();
    int prevStr = entityManager[playerIndex]->getStr();
    int prevDex = entityManager[playerIndex]->getDex();
    switch (attribute) {
    case '1':
        entityManager[playerIndex]->setInt(prevInt + 10);
        entityManager[playerIndex]->addSpells(3, 1);
        entityManager[playerIndex]->addSpells(2, 2);
        entityManager[playerIndex]->addSpells(0, 10);
        entityManager[playerIndex]->addSpells(1, 10);
        entityManager[playerIndex]->addSpells(4, 10);
        break;
    case '2':
        entityManager[playerIndex]->setStr(prevStr + 10);
        break;
    case '3':
        entityManager[playerIndex]->setDex(prevDex + 10);
        break;
    default:
        break;
    }
}


void GameField::update() {
    std::vector<int> enemyIndexes = entityManager.getIndexesWithEntity(Entity::entityType::ENEMY);
    int playerIndex = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    for (int index : enemyIndexes) {
        if (!entityManager[index]->alive()) {
            int enemyLevel = entityManager[index]->getLevel();
            cells[index].setAvaible(true);
            cells[index].setCellDead();
            entityManager[playerIndex]->addExperience(enemyLevel * enemyLevel * 10 + 10);
            entityManager.killEntity(index);
        }
    }
    std::vector<int> barrackIndexes = entityManager.getIndexesWithEntity(Entity::entityType::BARRACKS);
    for (int index : barrackIndexes) {
        if (!entityManager[index]->alive()) {
            int enemyLevel = entityManager[index]->getLevel();
            cells[index].setAvaible(true);
            cells[index].setCellDead();
            entityManager[playerIndex]->addExperience(enemyLevel * enemyLevel * 50 + 10);
            entityManager.killEntity(index);
        }
    }
    std::vector<int> towerIndexes = entityManager.getIndexesWithEntity(Entity::entityType::TOWER);
    for (int index : towerIndexes) {
        if (!entityManager[index]->alive()) {
            int enemyLevel = entityManager[index]->getLevel();
            cells[index].setAvaible(true);
            cells[index].setCellDead();
            entityManager[playerIndex]->addExperience(enemyLevel * enemyLevel * 50 + 10);
            entityManager.killEntity(index);
        }
    }
    std::vector<int> summonIndexes = entityManager.getIndexesWithEntity(Entity::entityType::SUMMONEDUNIT);
    for (int index : summonIndexes) {
        if (!entityManager[index]->alive()) {
            cells[index].setAvaible(true);
            cells[index].setCellDead();
            entityManager.killEntity(index);
        }
    }
}


int GameField::getBestTurnForEnemyPrimitive(int indexEnemy, int playerIndex) {
    Entity* ent = entityManager[indexEnemy];
    if (!ent) {
        return indexEnemy;
    }
    Enemy* enemy = dynamic_cast<Enemy*>(ent);
    if (!enemy) {
        return indexEnemy;
    }

    if (enemy->getIterative()) {
        std::vector<int> enemyTurns;
        enemyTurns.push_back(indexEnemy - widthField);
        enemyTurns.push_back(indexEnemy + widthField);
        enemyTurns.push_back(indexEnemy - 1);
        enemyTurns.push_back(indexEnemy + 1);
        std::vector<std::pair<int, float>> enemyTurnsWithDist = getDistanceToPlayer(enemyTurns, playerIndex);
        std::pair<int, float> bestTurn{indexEnemy, cells[indexEnemy].getDistance(cells[playerIndex])};
        for (const auto& [turn, dist] : enemyTurnsWithDist) {
            if (isMoveCorrect(indexEnemy, turn) && dist < bestTurn.second) {
                bestTurn.first = turn;
                bestTurn.second = dist;
            }
        }
        if (bestTurn.first == indexEnemy) {
            enemy->setIterative(false);
        }
        return bestTurn.first;
    } else {

        std::unordered_map<int, int> visited{};
        int res = getBestTurnForEnemyRecursive(indexEnemy, playerIndex, visited);

        return (res == -1) ? indexEnemy : res;
    }
}


int GameField::getBestTurnForEnemyRecursive(int indexEnemy, int playerIndex, std::unordered_map<int, int>& visited) {
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
        result = getBestTurnForEnemyRecursive(indexEnemy, up, visited);
        if (result != -1) return result;
    }
    if (isMoveCorrect(playerIndex, down) && !visited[down]) {
        result = getBestTurnForEnemyRecursive(indexEnemy, down, visited);
        if (result != -1) return result;
    }
    if (isMoveCorrect(playerIndex, left) && !visited[left]) {
        result = getBestTurnForEnemyRecursive(indexEnemy, left, visited);
        if (result != -1) return result;
    }
    if (isMoveCorrect(playerIndex, right) && !visited[right]) {
        result = getBestTurnForEnemyRecursive(indexEnemy, right, visited);
        if (result != -1) return result;
    }
    return -1;
}


std::vector<std::pair<int, float>>
GameField::getDistanceToPlayer(std::vector<int> enemyIndexes, int playerIndex) {
    std::vector<std::pair<int, float>> distances;
    float distance;
    for (long unsigned int i = 0; i < enemyIndexes.size(); ++i) {
        distance = cells[playerIndex].getDistance(cells[enemyIndexes[i]]);
        distances.push_back({enemyIndexes[i], distance});
    }
    return distances;
}

void GameField::enemyTurn() {
    int playerIndex = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];

    auto enemyIndexes = entityManager.getIndexesWithEntity(Entity::entityType::ENEMY);
    if (enemyIndexes.empty()) return;

    auto enemyIndexesWithDistances = getDistanceToPlayer(enemyIndexes, playerIndex);
    std::sort(enemyIndexesWithDistances.begin(), enemyIndexesWithDistances.end(),
              [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
                  return a.second > b.second;
              });

    std::unordered_set<int> occupiedNewIndices;

    for (const auto& [index, dist] : enemyIndexesWithDistances) {
        Entity* e = entityManager[index];
        if (!e) continue;
        if (!e->alive()) continue;
        // bool attacked = false;
        // std::vector<int> potentialTargets = {
        //     playerIndex,
        //     index - widthField, index + widthField,
        //     index - 1, index + 1
        // };

        // for (int targetIndex : potentialTargets) {
        //     if (cells[targetIndex].getDistance(cells[index]) <= 1) {
        //         Entity* target = entityManager[targetIndex];
        //         if (!target) continue;

        //         if (target->getType() == Entity::entityType::PLAYER ||
        //             target->getType() == Entity::entityType::SUMMONEDUNIT) {
        //             target->causeDamage(e->getDamage());
        //             attacked = true;
        //             break;
        //         }
        //     }
        // }

        // if (attacked) continue;
        if (cells[playerIndex].getDistance(cells[index]) <= 1) {
            Entity* playerEnt = entityManager[playerIndex];
            if (playerEnt) {
                playerEnt->causeDamage(e->getDamage());
            }
            continue;
        }

        int bestTurn = getBestTurnForEnemyPrimitive(index, playerIndex);
        bool isTrapped = cells[bestTurn].isTrapped();

        if (isTrapped) {
            int trapDamage = cells[bestTurn].checkAndSwitchTrap();
            e->causeDamage(trapDamage);
        }
        if (bestTurn == index) continue;
        if (occupiedNewIndices.count(bestTurn)) continue;

        if (isMoveCorrect(index, bestTurn)) {
            if (!entityManager[bestTurn] && cells[bestTurn].isCellAvaible()) {
                moveEntity(index, bestTurn);
                occupiedNewIndices.insert(bestTurn);
            }
        }
    }
}



void GameField::buildingsTurn() {
    int playerIndex = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    std::vector<int> barrackIndexes = entityManager.getIndexesWithEntity(Entity::entityType::BARRACKS);
    int index;
    if (!barrackIndexes.empty()) {
        index = barrackIndexes[0];
        if (entityManager[index]->timeToSpawn()) {
            int up = index - widthField;
            int down = index + widthField;
            int left = index - 1;
            int right = index + 1;

            if (isMoveCorrect(index, up)) {
                spawnEntity(std::make_unique<Enemy>(gameLevel), up);
            }
            else if (isMoveCorrect(index, down)) {
                spawnEntity(std::make_unique<Enemy>(gameLevel), down);
            }
            else if (isMoveCorrect(index, left)) {
                spawnEntity(std::make_unique<Enemy>(gameLevel), left);
            }
            else if (isMoveCorrect(index, right)) {
                spawnEntity(std::make_unique<Enemy>(gameLevel), right);
            }
        }
    }
    std::vector<int> towerIndexes = entityManager.getIndexesWithEntity(Entity::entityType::TOWER);
    int towerIndex;
    GameContext ctx{cells, entityManager};
    if (!towerIndexes.empty()) {
        towerIndex = towerIndexes[0];
        EnemyTower* tower = dynamic_cast<EnemyTower*>(entityManager[towerIndex]);
        if (cells[playerIndex].getDistance(cells[towerIndex]) <= tower->getSpellDistance()) {
            tower->towerAttack(ctx, towerIndex, 0);
        }
    }
}


std::unique_ptr<Entity> GameField::returnPlayer() {
    int playerIndex = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    return entityManager.returnEntity(playerIndex);
}


bool GameField::playerAlive() const {
    int playerIndex = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    return entityManager[playerIndex]->alive();
}


std::shared_ptr<PlayerData> GameField::getPlayerData() {
    int playerIndex = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    Player* player = dynamic_cast<Player*>(entityManager[playerIndex]);

    std::vector<long long> playerExperience = player->getExperience();
    int playerLevel = player->getLevel();
    bool playerLevelIncreased = player->isLevelIncreased();
    std::pair<int, int> playerHealth = player->getHealth();
    int playerAttack = player->getDamage();
    int playerIntelligence = player->getInt();
    int playerDexterity = player->getDex();
    int playerStrength = player->getStr();
    std::pair<int, int> playerHandSize = player->getHandSize();
    std::vector<std::pair<bool, int>> playerHandInfo = player->getHandItems();
    std::string playerWeapon;
    std::string playerDebaff;
    if (player->melle()) {
        playerWeapon = "Sword";
    }
    else {
        playerWeapon = "Bow";
    }
    if (player->checkDebaffState()) {
        player->setDebaffState();
        playerDebaff = "Slowed";
    }
    PlayerData* data = new PlayerData{
        playerExperience[0],
        playerExperience[1],
        playerExperience[2],
        playerLevel,
        playerLevelIncreased,
        playerHealth.second,
        playerHealth.first,
        playerAttack,
        playerIntelligence,
        playerDexterity,
        playerStrength,
        playerHandSize.first,
        playerHandSize.second,
        playerHandInfo,
        playerWeapon,
        playerDebaff
    };
    std::shared_ptr<PlayerData> dataSharedPtr(data);
    return dataSharedPtr;
}


std::vector<EnemyData> GameField::getEnemyData() {
    std::vector<int> playerIndexes = entityManager.getIndexesWithEntity(Entity::entityType::PLAYER);
    int playerIndex = playerIndexes[0];
    std::vector<int> enemyIndexes = entityManager.getIndexesWithEntity(Entity::entityType::ENEMY);
    if (enemyIndexes.empty()) {
        return {};
    }
    std::vector<std::pair<int, float>> enemyIndexesWithDistances = getDistanceToPlayer(enemyIndexes, playerIndex);
    std::sort(enemyIndexesWithDistances.begin(), enemyIndexesWithDistances.end(),
              [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
                  return a.second < b.second;
              });
    std::vector<EnemyData> data;
    data.reserve(enemyIndexesWithDistances.size());

    for (const auto& [index, dist] : enemyIndexesWithDistances) {
        Entity* ent = entityManager[index];
        if (!ent) continue;
        Enemy* enemy = dynamic_cast<Enemy*>(ent);
        if (!enemy) continue;
        EnemyData enemyData{};
        enemyData.enemyAttack = enemy->getDamage();
        std::pair<int, int> enemyHealth = enemy->getHealth();
        enemyData.enemyHealth = enemyHealth.first;
        enemyData.enemyMaxHealth = enemyHealth.second;
        enemyData.name = "Enemy";
        data.push_back(enemyData);
    }
    {
        std::vector<int> barracksIndexes = entityManager.getIndexesWithEntity(Entity::entityType::BARRACKS);
        if (!barracksIndexes.empty()) {
            int barracksIndex = entityManager.getIndexesWithEntity(Entity::entityType::BARRACKS)[0];
            Entity* ent = entityManager[barracksIndex];
            EnemyBarracks* baracks = dynamic_cast<EnemyBarracks*>(ent);
            EnemyData barracksData{};
            barracksData.enemyAttack = baracks->getDamage();
            std::pair<int, int> baracksHealth = baracks->getHealth();
            barracksData.enemyHealth = baracksHealth.first;
            barracksData.enemyMaxHealth = baracksHealth.second;
            barracksData.name = "Barrack";
            data.push_back(barracksData);
        }
    }
    {
        std::vector<int> towerIndexes = entityManager.getIndexesWithEntity(Entity::entityType::TOWER);
        if (!towerIndexes.empty()) {
            int towerIndex = entityManager.getIndexesWithEntity(Entity::entityType::TOWER)[0];
            Entity* ent = entityManager[towerIndex];
            EnemyTower* tower = dynamic_cast<EnemyTower*>(ent);
            EnemyData towerData{};
            towerData.enemyAttack = tower->getDamage();
            std::pair<int, int> towerHealth = tower->getHealth();
            towerData.enemyHealth = towerHealth.first;
            towerData.enemyMaxHealth = towerHealth.second;
            towerData.name = "Tower";
            data.push_back(towerData);
        }
    }
    return data;
}


std::vector<wchar_t> GameField::show() {
    std::vector<int> enemyIndexes = entityManager.getIndexesWithEntity(Entity::entityType::ENEMY);
    std::vector<wchar_t> data;
    for (int i = 0; i < widthField * heightField; ++i) {
        Entity* currentEntity = entityManager[i];
        if (cells[i].isCellAvaible() || currentEntity) {

            if (entityManager[i]) {
                if (currentEntity->getType() == Entity::entityType::PLAYER) {
                    data.push_back(L'𐇐');
                }
                else if (currentEntity->getType() == Entity::entityType::SUMMONEDUNIT) {
                    data.push_back(L'✦');
                }
                else if (currentEntity->getType() == Entity::entityType::ENEMY) {
                    data.push_back(L'𖨆');
                }
                else if (currentEntity->getType() == Entity::entityType::BARRACKS) {
                    data.push_back(L'🏟');
                }
                else if (currentEntity->getType() == Entity::entityType::TOWER) {
                    data.push_back(L'⛫');
                }
            }
            else if (cells[i].isCellSlow()) {
                data.push_back(L'░');
            }
            else if (cells[i].checkCellDead()) {
                data.push_back(L'☠');
            }
            else if (cells[i].isTrapped()) {
                data.push_back(L'᪠');
            }
            else {
                data.push_back('-');
            }
        }
        else {
            int randomRock = (9092/(i+1) << abs(i-900*i))/9*(i+1) % 4;
            switch (randomRock) {
            case 0:
                data.push_back(L'⛰');
                break;
            case 1:
                data.push_back(L'🏔');
                break;
            case 2:
                data.push_back(L'🌳');
                break;
            case 3:
                data.push_back(L'🌲');
                break;
            default:
                data.push_back(L'🏔');
                break;
            }
        }
    }
    return data;
}