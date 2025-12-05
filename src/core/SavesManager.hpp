#pragma once
#include "SaveData.hpp"
#include "FileHandler.hpp"
#include <iostream>
#include <filesystem>
#include <algorithm>

#define SAVEPATH "../saves/"


class SavesManager {
private:
    std::string getCorrectSaveName(std::string saveName);
public:
    SavesManager();
    ~SavesManager();
    void newSave(SaveData& data, std::string saveName);
    std::vector<std::string> getSavesList();
};
