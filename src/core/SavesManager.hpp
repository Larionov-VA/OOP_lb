#pragma once
#include "SaveData.hpp"
#include "FileHandler.hpp"
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <sstream>

#define SAVEPATH "../saves/"


class SavesManager {
private:
    std::string getCorrectSaveName(std::string saveName);
    std::string serializeData(SaveData& data);
    SaveData deserializeData(std::string& serializedData);
    std::string getStringFromEnemySaveData(std::vector<struct EnemySaveData>& enemySaveData);
    void parseEnemySaveData(std::stringstream& ss, std::vector<struct EnemySaveData>& enemySaveData);
    std::string getStringFromSpellSaveData(struct SpellSaveData& spellData);
    void parseSpellSaveData(std::stringstream& ss, SpellSaveData& spellData);
    std::string getStringFromCellsSaveData(std::vector<struct CellSaveData>& cellsData);
    void parseCellsSaveData(std::stringstream& ss, std::vector<struct CellSaveData>& cellsData);
public:
    SavesManager();
    ~SavesManager();
    void newSave(SaveData& data, std::string saveName);
    SaveData getLoadGameData(std::string saveName);
    std::vector<std::string> getSavesList(int start, int count);
};
