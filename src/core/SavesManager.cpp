#include "SavesManager.hpp"


SavesManager::SavesManager() {
    try {
        FileHandler file("SavesManager_log.txt", std::ios::out);
        for (const auto& entry : std::filesystem::directory_iterator(SAVEPATH)) {
            std::cout << entry.path() << std::endl;
            file.write(entry.path());
            file.write("\n");
        }
    } catch(...) {

    }
}


SavesManager::~SavesManager() {

}

std::string SavesManager::getCorrectSaveName(std::string saveName) {
    std::vector<std::string> savesList = getSavesList();

    auto it = std::find(savesList.begin(), savesList.end(), saveName);
    int counter = 1;
    FileHandler file("log_getCorrectSaveName", std::ios::app);
    if (it != savesList.end()) {
        file.write("it != savesList.end()");
        do {
            if (counter > 1) saveName.pop_back();
            saveName += std::to_string(counter++);
            it = std::find(savesList.begin(), savesList.end(), saveName);
        }
        while (it != savesList.end());
    }

    for (auto save: savesList) {
        file.write(save+'\n');
    }
    return saveName;
}


std::string SavesManager::getStringFromEnemySaveData(std::vector<struct EnemySaveData>& enemySaveData) {
    std::string stringData(std::to_string((int)enemySaveData.size()));
    for (auto enemyData : enemySaveData) {
        stringData += std::to_string(enemyData.enemyAttack.attack) + '\n';
        stringData += std::to_string(enemyData.enemyHealth.currentHealth) + '\n';
        stringData += std::to_string(enemyData.enemyHealth.maxHealth) + '\n';
        stringData += std::to_string(enemyData.enemyLevel) + '\n';
        stringData += std::to_string(enemyData.enemyIndex) + '\n';
    }
    return stringData;
}


std::string SavesManager::getStringFromSpellSaveData(struct SpellSaveData& spellData) {
    std::string stringData;
    stringData += spellData.countOfItem;
    stringData += spellData.powerOfSpell;
    stringData += spellData.baseDamage;
    stringData += spellData.baseDistance;
    stringData += spellData.trapLevel;
    stringData += spellData.trapDamage;
    return stringData;
}

std::string SavesManager::getStringFromCellsSaveData(std::vector<struct CellSaveData>& cellsData) {
    std::string stringData(std::to_string((int)cellsData.size()));
    for (auto& cell : cellsData) {
        stringData += std::to_string(cell.index) + '\n';
        stringData += std::to_string(cell.X) + '\n';
        stringData += std::to_string(cell.Y) + '\n';
        stringData += std::to_string(cell.stateData.avaible) + '\n';
        stringData += std::to_string(cell.stateData.haveConstState) + '\n';
        stringData += std::to_string(cell.stateData.haveTempState) + '\n';
        if (cell.stateData.haveConstState) {
            stringData += std::to_string(cell.stateData.constState.durationOfState) + '\n';
            stringData += std::to_string(cell.stateData.constState.damage) + '\n';
            stringData += std::to_string(cell.stateData.constState.stateSymbol) + '\n';
        }
        else {
            stringData += std::to_string(0) + '\n';
            stringData += std::to_string(0) + '\n';
            stringData += std::to_string(0) + '\n';
        }
        if (cell.stateData.haveTempState) {
            stringData += std::to_string(cell.stateData.tempState.durationOfState) + '\n';
            stringData += std::to_string(cell.stateData.tempState.damage) + '\n';
            stringData += std::to_string(cell.stateData.tempState.stateSymbol) + '\n';
        }
        else {
            stringData += std::to_string(0) + '\n';
            stringData += std::to_string(0) + '\n';
            stringData += std::to_string(0) + '\n';
        }
    }
    return stringData;
}


std::string SavesManager::serializeData(SaveData& data) {
    SaveData saveData = data;
    FieldSaveData fieldSaveData = data.fieldData;
    EntitySaveData entitySaveData = fieldSaveData.entityData;
    PlayerSaveData playerSaveData = entitySaveData.playerData;
    AttackSaveData playerAttackSaveData = playerSaveData.playerAttack;
    AttributesSaveData playerAttributesSaveData = playerSaveData.playerAttributes;
    EquipmentSaveData playerEquipmentSaveData = playerSaveData.playerEquipment;
    HandSaveData playerHandSaveData = playerSaveData.playerHand;
    HealthSaveData playerHealthSaveData = playerSaveData.playerHealth;
    StatsSaveData playerStatsSaveData = playerSaveData.playerStats;
    BarrackSaveData barrackSaveData = entitySaveData.barrackData;
    HealthSaveData barrackHealthSaveData = barrackSaveData.barracksHealth;
    TowerSaveData towerSaveData = entitySaveData.towerData;
    HealthSaveData towerHealthSaveData = towerSaveData.towerHealth;
    std::string stringData(
            std::to_string(saveData.gameID) + '\n' +
            std::to_string(fieldSaveData.widthField) + '\n' +
            std::to_string(fieldSaveData.heightField) + '\n' +
            std::to_string(fieldSaveData.gameLevel) + '\n' +
            std::to_string(fieldSaveData.gameTurn) + '\n' +
            std::to_string(playerAttackSaveData.attack) + '\n' +
            std::to_string(playerAttributesSaveData.intelligence) + '\n' +
            std::to_string(playerAttributesSaveData.dexterity) + '\n' +
            std::to_string(playerAttributesSaveData.strength) + '\n' +
            std::to_string(playerEquipmentSaveData.currentWeapon) + '\n' +
            std::to_string(playerEquipmentSaveData.meleeWeaponMulti) + '\n' +
            std::to_string(playerEquipmentSaveData.rangeWeaponMulti) + '\n' +
            std::to_string(playerHandSaveData.itemInHand) + '\n' +
            std::to_string(playerHandSaveData.powerUp) + '\n' +
            std::to_string(playerHandSaveData.maxSize) + '\n' +
            std::to_string(playerHandSaveData.currentSize) + '\n' +
            getStringFromSpellSaveData(playerHandSaveData.areaSpell) + '\n' +
            getStringFromSpellSaveData(playerHandSaveData.directSpell) + '\n' +
            getStringFromSpellSaveData(playerHandSaveData.updateSpell) + '\n' +
            getStringFromSpellSaveData(playerHandSaveData.trapSpell) + '\n' +
            std::to_string(playerHealthSaveData.currentHealth) + '\n' +
            std::to_string(playerHealthSaveData.maxHealth) + '\n' +
            std::to_string(playerStatsSaveData.prevLevelUpExperience) + '\n' +
            std::to_string(playerStatsSaveData.currentExperience) + '\n' +
            std::to_string(playerStatsSaveData.levelUpExperience) + '\n' +
            std::to_string(playerStatsSaveData.level) + '\n' +
            std::to_string(playerStatsSaveData.levelIncreased) + '\n' +
            std::to_string(playerSaveData.slowed) + '\n' +
            getStringFromEnemySaveData(entitySaveData.enemyData) + '\n' +
            std::to_string(barrackSaveData.spawnPeriod) + '\n' +
            std::to_string(barrackSaveData.barracksLevel) + '\n' +
            std::to_string(barrackSaveData.counter) + '\n' +
            std::to_string(barrackHealthSaveData.currentHealth) + '\n' +
            std::to_string(barrackHealthSaveData.maxHealth) + '\n' +
            std::to_string(towerSaveData.attackPeriod) + '\n' +
            std::to_string(towerSaveData.attackCooldown) + '\n' +
            std::to_string(towerSaveData.towerlevel) + '\n' +
            std::to_string(towerHealthSaveData.currentHealth) + '\n' +
            std::to_string(towerHealthSaveData.maxHealth) + '\n' +
            getStringFromSpellSaveData(towerSaveData.towerSpell) + '\n' +
            getStringFromCellsSaveData(fieldSaveData.cellsData)
    );
    return stringData;
}


void SavesManager::newSave(SaveData& data, std::string saveName) {
    std::cout << saveName;
    try {
        std::string fullSavePath(SAVEPATH + saveName);
        std::string correctSaveName = getCorrectSaveName(fullSavePath);
        FileHandler file(fullSavePath, std::ios::out);
        file.write(saveName);
        file.write(serializeData(data));
    } catch(...) {

    }
}

std::vector<std::string> SavesManager::getSavesList() {
    std::vector<std::string> savesList;
    for (const auto& entry : std::filesystem::directory_iterator(SAVEPATH)) {
        savesList.push_back(entry.path().string());
    }
    return savesList;
}