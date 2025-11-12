#pragma once
#include <filesystem>
#include "FileHandler.hpp"
#include "ISaveManager.hpp"
#include "SavesTree.hpp"
#define SAVES_PATH "../saves/"

class SaveManager {
private:
    void createDirTree(int gameID);
    // void saveRecursive(int gameID, ISaveManager* headOfTree);
public:
    void newSaveDir(int gameID);
    SaveManager();
};