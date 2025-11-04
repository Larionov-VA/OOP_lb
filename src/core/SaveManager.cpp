#include "SaveManager.hpp"


SaveManager::SaveManager() {
    std::filesystem::path saveDir = PATH;
    std::filesystem::create_directory(saveDir);
}


SaveManager::~SaveManager() {

}


void SaveManager::newSave(int gameID) {
    std::filesystem::path newSaveDir = PATH + std::to_string(gameID);
    std::filesystem::create_directory(newSaveDir);
}