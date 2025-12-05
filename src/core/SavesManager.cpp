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
    if (it != savesList.end()) {
        do {
            saveName += std::to_string(counter++);
            it = std::find(savesList.begin(), savesList.end(), saveName);
        }
        while (it != savesList.end());
    }
    return saveName;
}

void SavesManager::newSave(SaveData& data, std::string saveName) {
    try {
        std::string correctSaveName = getCorrectSaveName(saveName);
        std::string fullSavePath(SAVEPATH + correctSaveName);
        FileHandler file(fullSavePath, std::ios::out);
        file.write(std::to_string(data.gameID) + '\n' + std::to_string(data.fieldData.heightField));
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